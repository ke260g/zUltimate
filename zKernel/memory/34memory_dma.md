[TOC]
# 1 DMA
+ DMA 在驱动层需要做的业务; 本质上就是使用 内核接口 与 DMA 器件通讯
+ DMA 器件可以抽象为两类: PCI(直连内存条) 和 ISA(BUS共享DMA)
## 1.1 设计模型
### 1.1.1 同步 DMA read模型
1. 进程发起read请求(read or ioctl+mmap); 阻塞进入内核态; 
2. 驱动申请一块内存(可能是已经分配好的);
3. 驱动通知硬件进行读操作, 数据缓存地址为申请的内存; 
4. 硬件执行读操作 完成后触发 **读完成-中断**
5. 驱动在**读完成-中断**回调 中获取到硬件数据; 唤醒进程
6. 进程读取read请求 通过 read or mmap 取得数据
### 1.1.2 异步 DMA read模型
1. 硬件产生新的数据 触发 **新数据-中断**
2. 驱动在中断回调a中 申请内存用以存放 硬件的数据; 通知硬件内存准备完成
3. 硬件把新的数据写入 刚申请的内存; 触发 **读完成-中断**
4. 驱动在终端回调b中 通知进程有新数据可用  

## 1.2 DMA 使用步骤
### 1.2.1 驱动申请
+ 只有特定的内存地址空间 才可以进行DMA
+ 内核接口`kmalloc()` `get_free_pages()` 参数flags加上GFP_DMA

### 1.2.2 内存地址
+ 实际上; 硬件连接着 内存中特殊的位置 bus-address (内核中也表现为phys地址)
+ 代码上: 可以使用 `phys_to_*()` `*_to_phys()` 接口

## 1.3 The Generic DMA Layer
1. 内核封装了一套接口; 用以解决一下问题
    1. 不同 arch 的 cache coherenc 工作机制不同
    2. 不同 arch 硬件上有着不一样的bus结构
    3. 有些 arch 没有 DMA 功能
2. 驱动层不建议用 `virt_to_bus`; 有些硬件带了 IOMMU 通过一系列 "映射寄存器" 控制bus地址
3. bounce buffer
4. cache coherency: 缓存连贯性
5. `dma_addr_t` 表示总线地址: 只提供给硬件和其他DMA-routines; 不能被CPU直接访问

### 1.3.1 设置地址长度 dma_set_mask
+ 默认dma寻址空间是 32bits; 如果设备需要24bits、12bits等则需要通知内核
```c++ 
/**
 * @mask: 如果是 24bits 则 0xffffff; 如果是 12bits: 则 0xfff
 */
int dma_set_mask(struct device *dev, u64 mask);
```

# 2. PCI DMA (x86)
## 2.1 Coherent DMA mappings  (dma_coherent / dma_pools)
1. 特性
    1. 该映射类型一般 伴随一整个驱动的生命周期
    2. 允许同时被 CPU 和 peripheral 访问, 即只能存放于 cache-coherent 内存中,
    3. 访问和配置时会有较大的性能消耗(所以一次配置; 长期使用)
2. 类型
    1. coherent:
    2. pool:
3. 接口
```c++
void *dma_alloc_coherent(struct device *dev, size_t size, dma_addr_t *dma_handle, gfp_t gfp);
void dma_free_coherent(struct device *dev, size_t size, void *vaddr, dma_addr_t dma_handle);

struct dma_pool *dma_pool_create(const char *name, struct device *dev, size_t size, size_t align, size_t allocation);
void dma_pool_destroy(struct dma_pool *pool);
/**
 * @mem_flags: GFP_* 内存申请标识
 */
void *dma_pool_alloc(struct dma_pool *pool, gfp_t mem_flags, dma_addr_t *handle);
void dma_pool_free(struct dma_pool *pool, void *vaddr, dma_addr_t addr);
```

## 2.2 Streaming DMA mappings
1. 特性
    1. 该映射用于硬件已经分配好的地址; 所以要处理硬件没有 choose 的地址部分
    2. 有些架构的 stream-dma 是由一系列不连续的 page 构成
    3. 有些架构的 stream-dma 是由多个 "scatter/gather" buffers 组成
2. 使用注意事项:
    1. dma_data_direction 的使用方向必须匹配
    2. `dma_map_*`  映射后内存只由硬件读写; cpu不能读写; 除非使用 `dma_unmap_*` / `dma_sync_single_for_cpu` 操作
    3. `dma_unmap_*` 调用前必须保证硬件没有对该块内存进行操作
2. 类型:
    1. single / page / scatterlist
3. 接口
```c++
// 实际上可以都使用 bidirectional; 只是有些arch对 to_device; from_device会优化
enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,        // cpu -> device
	DMA_FROM_DEVICE = 2,      // device -> cpu
	DMA_NONE = 3,             // 只用于debug; 运行中使用会产生内核panic
};

// 映射一块内存 ==============================
dma_addr_t dma_map_single(struct device *dev, void *ptr,
    size_t size, enum dma_data_direction direction);
void dma_unmap_single(struct device *dev, dma_addr_t dma_addr,
    size_t size, enum dma_data_direction direction);
// 把dma的content 从硬件同步到CPU 
// 调用前需要保证硬件停止 DMA的操作; 调用后CPU允许访问
void dma_sync_single_for_cpu(struct device *dev, dma_addr_t dma_handle,
    size_t size, enum dma_data_direction direction);
// 把dma的content 重新同步到硬件
void dma_sync_single_for_device(struct device *dev, dma_addr_t dma_handle,
    size_t size, enum dma_data_direction direction);
//////////////////////////////////////////////

// 映射 page ==============================
// 一般 offset 是0, size 是 PAGE_SIZE; 否则驱动需要处理缓存连贯性问题
dma_addr_t dma_map_page(struct device *dev, struct page *page,                                unsigned long offset, size_t size, enum dma_data_direction direction);
void dma_unmap_page(struct device *dev, dma_addr_t dma_address,
    size_t size, enum dma_data_direction direction);
//////////////////////////////////////////////

// 映射 矢量; 形同 readv / writev ========
// 1.1 构造scatterlist::初始化一个 array 
void sg_init_table(struct scatterlist *sgl, unsigned int nents);

// 1.2 设置映射 page;
// 一般 offset 是0, len 是 PAGE_SIZE; 否则驱动需要处理缓存连贯性问题
void sg_set_page(struct scatterlist *sg, struct page *page,
    unsigned int len, unsigned int offset);

// 2. 映射 / 反映射 DMA
int dma_map_sg(struct device *dev, struct scatterlist *sg, int nents,
    enum dma_data_direction direction);
void dma_unmap_sg(struct device *dev, struct scatterlist *sg, int nhwentries,
    enum dma_data_direction direction);

// 3. CPU 占用 / 释放
void dma_sync_sg_for_cpu(struct device *dev, struct scatterlist *sg, int nelems,
    enum dma_data_direction direction);
void dma_sync_sg_for_device(struct device *dev, struct scatterlist *sg, int nelems,
    enum dma_data_direction direction);

// 4. CPU 读写; 相当于 readv / writev
size_t sg_copy_from_buffer(struct scatterlist *sgl, unsigned int nents,
    void *buf, size_t buflen);
size_t sg_copy_to_buffer(struct scatterlist *sgl, unsigned int nents,
    void *buf, size_t buflen);
//////////////////////////////////////////
```
4. 使用
+ 映射 -> 触发硬件操作 -> 硬件操作完成的中断回调中反映射
```c++
int dad_transfer(struct dad_dev *dev, int write, void *buffer, size_t count){
    dma_addr_t bus_addr;
    int dma_dir =  (write ? DMA_TO_DEVICE : DMA_FROM_DEVICE);
    /* Map the buffer for DMA */ // 执行映射; 关键一步
    bus_addr = dma_map_single(&dev->pci_dev->dev, buffer, count, dma_dir);

    // 存储一些信息
    dev->dma_dir = dma_dir;
    dev->dma_size = count;
    dev->dma_addr = bus_addr;

    /* Set up the device */ // 硬件的业务逻辑
    writeb(dev->registers.command, DAD_CMD_DISABLEDMA);
    writeb(dev->registers.command, write ? DAD_CMD_WR : DAD_CMD_RD);
    writel(dev->registers.addr, cpu_to_le32(bus_addr));
    writel(dev->registers.len, cpu_to_le32(count));
    /* Start the operation */
    writeb(dev->registers.command, DAD_CMD_ENABLEDMA);
    return 0;
}

void dad_interrupt(int irq, void *dev_id, struct pt_regs *regs) {
    struct dad_dev *dev = (struct dad_dev *) dev_id;

    /* Make sure it's really our device interrupting */
    // pci 是共享中断; 这里需要确认是否是 本device的中断

    /* Unmap the DMA buffer */
    dma_unmap_single(dev->pci_dev->dev, dev->dma_addr,
        dev->dma_size, dev->dma_dir);

    /* Only now is it safe to access the buffer, copy to user, etc. */
    // ...
}
```

# 3 ISA DMA (x86)
## 3.1 native DMA
+ 使用标准的 DMA控制器 在母板上驱动 isa的信号线
+ 实际上 ISA 是共享bus; 同一时间只有一个设备传输数据;
+ 由于是共享bus; 所以 request_irq 必须在 open 时与 request_dma 先后执行
    + 否则; 设备初始化 request_irq 导致ISA的其他设备中断冲突
+ open  中 request_irq 必须先于 request_dma; 
    + 如果 先使能了 dma; 设备有数据产生的中断 驱动层丢了没处理; 可能导致硬件死锁
+ close 中 free_dma    必须先于 free_irq
    + 如果 先把irq 停掉; 设备有数据产生的中断 驱动层丢了没处理; 可能导致硬件死锁
+ 组件
    1. The 8237 DMA Controller (DMAC) = DMAC的管理 硬件; 负责信号触发/数据传输
    2. The peripheral device = 挂载在 DMAC 上的具体设备
    3. The device driver = 与DMAC通讯; 告知 bus_addr / dmac_direction / buff_size; 响应中断

### 3.1.1 接口
```c++
// 申请
/**
 * @channel: 大于 0; 小于 MAX_DMA_CHANNELS
 * @name:    /proc/dma 中的entry
 */
int request_dma(unsigned int channel, const char *name);
void free_dma(unsigned int channel);
////////////////////////////////////////////////////////////

// ISA 是共享bus; 单个设备驱动对 DMAC 的操作需要加锁
unsigned long claim_dma_lock( ); // 返回的是 spin_lock_irqsave 的flags
void release_dma_lock(unsigned long flags); // 传入 claim_dma_lock 返回值
////////////////////////////////////////////////////////////

// 与 DMAC 通讯; 告知 bus_addr / dmac_direction / buff_size;
/**
 * @mode: DMA_MODE_READ / DMA_MODE_WRITE / DMA_MODE_CASCADE(释放对dma bus的占用)
 */
void set_dma_addr(unsigned int channel, unsigned int addr);   // bus_addr (特殊的virt_addr)
void set_dma_mode(unsigned int channel, char mode);
void set_dma_count(unsigned int channel, unsigned int count);
////////////////////////////////////////////////////////////

// 使能
void enable_dma(unsigned int channel);
void disable_dma(unsigned int channel);
////////////////////////////////////////////////////////////

// 其他
int get_dma_residue(unsigned int channel); // 还有多少dma数据需要传输
/**
 * @brief 关掉 flip-flop 寄存器
 * @note  flip-flop 寄存器用于指示 DMAC 使用 8bits 寻址; 关掉表示使用 16bits
 *        由于 set_dma_* 使用16bits; 所以 set_dma_* 必须 clear_dma_ff
 */
void clear_dma_ff(unsigned int channel);   // ??
////////////////////////////////////////////////////////////
```

### 3.1.2 使用
```c++
int dad_open(struct inode *inode, struct file *filp) {
    struct dad_device *my_device;
    /* ... */
    if ((error = request_irq(my_device.irq, dad_interrupt, SA_INTERRUPT, "dad", NULL)) )
        return error; /* or implement blocking open */
    if ( (error = request_dma(my_device.dma, "dad")) ) {
        free_irq(my_device.irq, NULL);
        return error; /* or implement blocking open */
    }
    /* ... */
    return 0;
}

void dad_close(struct inode *inode, struct file *filp){
    struct dad_device *my_device;
    /* ... */
    free_dma(my_device.dma);
    free_irq(my_device.irq, NULL);
    /* ... */
}

// 数据传输
int dad_dma_prepare(int channel, int mode,
    unsigned int buf, unsigned int count) {
    unsigned long flags;
    flags = claim_dma_lock( );
    disable_dma(channel);
    clear_dma_ff(channel);
    set_dma_mode(channel, mode);
    set_dma_addr(channel, virt_to_bus(buf));
    set_dma_count(channel, count);
    enable_dma(channel);
    release_dma_lock(flags);
    return 0;
}

// 检查还剩多少数据
int dad_dma_isdone(int channel) {
    int residue;
    unsigned long flags = claim_dma_lock ( );
    residue = get_dma_residue(channel);
    release_dma_lock(flags);
    return (residue = = 0);
}
```

## 3.2 ISA bus master DMA
+ 完全由 peripheral device 控制; 形同 PCI DMA
+ 例子 `drivers/scsi/aha1542.c`