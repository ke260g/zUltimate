unreferenced object 0xffff80022a7c0000 (size 10368):
  comm "iplink", pid 1313, jiffies 4294893934 (age 71392.552s)
  hex dump (first 32 bytes):
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
    00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
  backtrace:
    [<ffff00000818922c>] kmalloc_order+0x44/0x58
    [<ffff0000081bc864>] __kmalloc+0x1fc/0x240
    [<ffff0000084febdc>] mvpp2_frag_alloc.isra.65+0x2c/0x38
    [<ffff0000084ff86c>] mvpp2_bm_pool_use+0x10c/0x3f0
    [<ffff0000084ffc70>] mvpp2_swf_bm_pool_init+0x120/0x1d0
    [<ffff0000084ffe1c>] mvpp2_bm_update_mtu+0xfc/0x178
    [<ffff000008502948>] mvpp2_change_mtu+0x38/0x1a8
    [<ffff0000085eed44>] dev_set_mtu+0xe4/0x1b0
    [<ffff000008618c30>] dev_ifsioc+0x330/0x3e0
    [<ffff000008618f78>] dev_ioctl+0x298/0x900
    [<ffff0000085cd734>] sock_do_ioctl+0x64/0x78
    [<ffff0000085ceca4>] sock_ioctl+0x26c/0x3b0
    [<ffff0000081ebe14>] do_vfs_ioctl+0xa4/0x9f0
    [<ffff0000081ec7d8>] SyS_ioctl+0x78/0x98
    [<ffff000008083a00>] el0_svc_naked+0x34/0x38
    [<ffffffffffffffff>] 0xffffffffffffffff

ip link set eth0 mtu 1500

堆栈
mvpp2_frag_alloc
mvpp2_buf_alloc
mvpp2_bm_bufs_add
mvpp2_bm_pool_use
mvpp2_swf_bm_pool_init
mvpp2_bm_update_mtu
mvpp2_change_mtu

在 mvpp2_buf_alloc 里面
	data = mvpp2_frag_alloc(bm_pool); // 申请了内存
	dma_addr = dma_map_single(port->dev->dev.parent, data,
				  bm_pool->buf_size, DMA_FROM_DEVICE); // 这里放到了 dma 里面
	return dma_addr;
在 mvpp2_bm_bufs_add 里面
	dma_addr = mvpp2_buf_alloc(port, bm_pool, GFP_KERNEL); // 拿到了dma_addr, 显然不同于最开始kmalloc的地址
	mvpp2_bm_pool_put(port, bm_pool->id, dma_addr);        // 



释放的逻辑
mvpp2_frag_free(bm_pool, data);  <-- 该函数有4个引用, 其他3个都是错误处理, 只有 mvpp2_bm_bufs_free 是正常处理
	mvpp2_bm_bufs_get_addrs(dev, priv, bm_pool,
			&buf_dma_addr, &buf_phys_addr); // 在一个priv的bm_pool中 获取 dma_addr, phys_addr
	dma_unmap_single(dev, buf_dma_addr,
			 bm_pool->buf_size, DMA_FROM_DEVICE); // 反映射dma
	data = (void *)phys_to_virt(buf_phys_addr);   // 物理地址转换为虚拟地址
	mvpp2_frag_free(bm_pool, data);               // 释放虚拟地址
mvpp2_bm_bufs_free;  <-- 引用者  mvpp2_bm_pool_use; mvpp2_bm_pool_destroy


mvpp2_bm_pool_use {
	if (new_pool->pkt_size == 0) {
		int pkts_num;

		/* Set default buffer number or free all the buffers in case
		 * the pool is not empty
		 */
		pkts_num = new_pool->buf_num;
		if (pkts_num == 0) {
			pkts_num = mvpp2_bm_pool_default_buf_num(pool_type);
			if (pkts_num < 0)
				return NULL;
		} else {
			mvpp2_bm_bufs_free(port->dev->dev.parent,
					   port->priv, new_pool, pkts_num);     <--- 理论上跑到这里会释放上一个 mvpp2_bm_bufs_add 的内存
		}
		/* Allocate buffers for this pool */
		num = mvpp2_bm_bufs_add(port, new_pool, pkts_num);  <--- 要等到下一次才会释放
	}
}

iplink 那块内存泄漏是误判。
1. mvpp2设置mtu时;  kmalooc申请一块内存; 然后 dma_map_single;
   驱动上下文只保存了dma的地址; 没有保存 kmalloc的虚地址; 所以误判。 
2. 这块内存时常驻; 在更新时会释放上一次的申请的内存
3. 释放的逻辑是 把dma_addr映射为phys_addr, 再把映射为首次申请的地址 最后释放。
申请= drivers\net\ethernet\marvell\mvpp2\mvpp2_main.c::mvpp2_buf_alloc
释放= drivers\net\ethernet\marvell\mvpp2\mvpp2_main.c::mvpp2_bm_pool_use
      drivers\net\ethernet\marvell\mvpp2\mvpp2_main.c::mvpp2_bm_bufs_free

每个网卡对象, 有3个pool,
mvpp2_bm_pools_init() 中初始化了这三个pool
每个pool 有固定大小的 bufs[]
```c++
enum mvpp2_bm_pool_log_num {
	MVPP2_BM_SHORT,
	MVPP2_BM_LONG,
	MVPP2_BM_JUMBO,
	MVPP2_BM_POOLS_NUM
};

mvpp2_bm_pools_init() {
    for (i = 0; i < MVPP2_BM_POOLS_NUM; i++) {
        bm_pool = &priv->bm_pools[i];
        err = mvpp2_bm_pool_create(pdev, priv, bm_pool, size);
    }
}
```

每个 buf 后面会 kmalloc 申请; 然后 dma_single_map

## 提交方案
```c++
#define MVPP2_BM_JUMBO_BUF_NUM		512
#define MVPP2_BM_LONG_BUF_NUM		1024
#define MVPP2_BM_SHORT_BUF_NUM		2048
#define MVPP2_BM_SHORT_BUF_NUM		2048
#define MVPP2_BM_MAX_BUF_NUM      MVPP2_BM_SHORT_BUF_NUM // 最大的 BUF_NUM
struct mvpp2_bm_pool {
    /* frag of each buf */
    void *buf_frag[MVPP2_BM_MAX_BUF_NUM]; // 每个池, 用数组保存 kmalloc 出来的内存地址
};

static dma_addr_t mvpp2_buf_alloc(struct mvpp2_port *port,
 				  struct mvpp2_bm_pool *bm_pool,
				  gfp_t gfp_mask, void **frag); // 返回 kmalloc 出来的地址到 frag

static int mvpp2_bm_bufs_add(struct mvpp2_port *port,
			     struct mvpp2_bm_pool *bm_pool, int buf_num) {
    for (i = 0; i < buf_num; i++) {
        dma_addr = mvpp2_buf_alloc(port, bm_pool, GFP_KERNEL, &frag);
        if (!dma_addr)
          break;

        mvpp2_bm_pool_put(port, bm_pool->id, dma_addr);
        bm_pool->buf_frag[i] = frag; // <---- 保存起来
    }
}
```

## 历史尝试1 (kmemleak_ignore / kmemleak_no_scan)
使用 kmemleak_ignore 或者 kmemleak_no_scan 是不行的.
mvpp2_buf_alloc 中, 调用 内存申请时, 不是每次返回的都受到 kmemleak 监管

因为 mvpp2_frag_alloc 接口会调用 netdev_alloc_frag 分配内存
netdev_alloc_frag 有时候不需要申请新的内存, 且不一定返回申请的内存指针
netdev_alloc_frag 的机制
    用于分配小于整页的内存时, 当缓存足够大, 不需要真实意义上的分配内存(不受kmemleak监管)
    当缓存不够大, 那就分配整页的内存(这个内存 受到kmemleak监管)
    所以申请的内存指针是指向页头的,
    但 netdev_alloc_frag 返回的指针是页中间的

```c++
static void *mvpp2_frag_alloc(const struct mvpp2_bm_pool *pool) {
	if (likely(pool->frag_size <= PAGE_SIZE))
		return netdev_alloc_frag(pool->frag_size);
	else
		return kmalloc(pool->frag_size, GFP_ATOMIC);
}
```
