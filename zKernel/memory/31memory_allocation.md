[TOC]
kmalloc looks rather different from a typical user-spacemalloc implementation. 
A simple, heap-oriented allocation technique would quickly run into trouble;
it would have a hard time working around the page boundaries.

Linux handles memory allocation by creating a set of pools of memory objects offixed sizes. 
Allocation requests are handled by going to a pool that holds sufficiently large objects and 
handing an entire memory chunk back to the requester. 

The kernel canallocate  only certain predefined,  fixed-size byte  arrays.
The  smallest  allocation  thatkmalloccan handle is as big as 32 or 64 bytes, depending on the page size used bythe system’s architecture.

# 1, kmalloc
+ flag's feature
+ flag's applicable scene
+ 3 zone of memory: DMA-capablememory; normal memory; high memory;
1. `void *kmalloc(size_t size, int flags);`
2. mainly flag = GFP_ATOMIC/GFP_KERNEL/GFP_USER
3. ORing flag
    1. __GFP_DMA/__GFP_HIGHMEM
    2.  __GFP_COLD/__GFP_NOWARN
    3. __GFP_HIGH
    3. __GFP_REPEAT/__GFP_NOFAIL/__GFP_NORETRY

| flag | feature | scene |
| --:  | :-: | :-- |
| GFP_ATOMIC | Never Sleep | interrupt; outside of a process context |
| GFP_KERNEL | May Sleep   | Most normal allocation |
| GFP_USER   | May Sleep   | allocates user-space pages |

## 1.1 其他变体接口
+ kvzalloc(); 内嵌__GFP_ZERO 标记; 申请初始化为 0 的内存. 调用者如 `alloc_etherdev()`

# 2. kmem_*（memory pool) /proc/slabinfo
```c++
struct kmem_cache *kmem_cache_create(const char *name, 
            size_t size, size_t align,
            
            unsigned long flags, void (*ctor)(void *));
void kmem_cache_destroy(struct kmem_cache *s);
int kmem_cache_shrink(struct kmem_cache *cachep);

void *kmem_cache_alloc(struct kmem_cache *, gfp_t flags);
void kmem_cache_free(struct kmem_cache *c, void *b);
```
1. The cache keeps a pointer to the name, rather than copying it, 
so the driver should pass in apointer to a name in static storage (usually the name is just a literal string). 
2. The name cannot contain blanks
3. each cache can be traced in /proc/slabinfo

# 3. mempool (interface wrapper of kmem_*)
## 3.1 接口
```c++
mempool_t *mempool_create(int min_nr, mempool_alloc_t *alloc_fn,
				mempool_free_t *free_fn, void *pool_data);
int mempool_resize(mempool_t *pool, int new_min_nr, gfp_t gfp_mask);
void mempool_destroy(mempool_t *pool);

extern void *mempool_alloc(mempool_t *pool, gfp_t gfp_mask); // shadowly call alloc_fn
extern void mempool_free(void *element, mempool_t *pool);    // shadowly call free_fn
```
## 3.2 实现
## 3.3 使用 (drivers/scsi/storvsc_drv.c)
```c++
pool_data = kmem_cache_create();
pool = mempool_create(..., pool_data);
```

# 4, get_zeroed_page (big chucks of memory)
## 4.1 接口
```c++
/**
 * @note the amount of returned pages is 2^order;
 *       for example return 1 page when `order' is zero.
 *                   return 8 page when `order' is three.
 *       the maximum of order is the columns of /proc/buddyinfo
 */
unsigned long __get_free_pages(gfp_t gfp_mask, unsigned int order);
unsigned long get_zeroed_page(gfp_t gfp_mask);
#define __get_free_page(gfp_mask) __get_free_pages((gfp_mask), 0)
#define __get_dma_pages(gfp_mask, order) __get_free_pages((gfp_mask) | GFP_DMA, (order))

#define alloc_pages(gfp_mask, order)
#define alloc_pages_vma(gfp_mask, order, vma, addr, node)
#define alloc_page(gfp_mask) alloc_pages(gfp_mask, 0)
#define alloc_page_vma(gfp_mask, vma, addr)	 alloc_pages_vma(gfp_mask, 0, vma, addr, numa_node_id())

void __free_pages(struct page *page, unsigned int order);
void free_pages(unsigned long addr, unsigned int order);
#define __free_page(page) __free_pages((page), 0)
#define free_page(addr) free_pages((addr), 0)
```
## 4.2 实现
## 4.3 使用

# 5. vmalloc / 
+ 尽量少用; 耗时较长
+ {`kmalloc` `__get_free_pages`} 和 `vmalloc` 分配的都是虚拟内存; 需要MMU到物理内存
+ {`kmalloc` `__get_free_pages`} 和 `vmalloc` 分配的算法不同; 导致`vmalloc`性能不好
    1. 简单理解: `kmalloc` 分配的是已经建立好 "物理-虚拟"映射关系内存
    2. `vmalloc` 分配的是还没建立 "物理-虚拟"映射关系的内存; 需要在`vmalloc`中建立
    3. 实际上 `kmalloc` 申请前; 还是需要建立超大一片的内存的"物理-虚拟"映射关系
    4. "物理-虚拟"映射关系即 内核中的"pages-tables"
+ 不能在 atomic-context 使用; 因为构建 "pages-tables" 会休眠
## 5.1 接口
```c++
void *vmalloc(unsigned long size);
void vfree(const void *addr);
```
## 5.2 实现
## 5.3 使用

# 6. ioremap
1. ioremap 把物理地址映射到内存地址
2. 某些arch在一些场景中物理地址不应该被直接使用; 应该通过`$arch/include/asm/io.h`中的方法读写
## 6.1 接口
```c++
void __iomem *ioremap(phys_addr_t offset, unsigned long size);
void iounmap(void __iomem *addr);
```

# 7. per_cpu
## 7.1 接口 （定义/读/写)
```c++
// 定义
#define DEFINE_PER_CPU(type, name)
void *alloc_percpu(type);

// 读
#define get_cpu_var(var)
#define per_cpu_ptr(ptr, cpu) // 这个使用比较特殊

#define put_cpu_var(var) // 写


```
## 7.2 实现
## 7.3 使用
### 7.3.1 per_cpu_ptr
```c++
// per_cpu_ptr 使用例子1 遍历CPU
// net/core/neighbour.c::neightbl_fill_info
for_each_possible_cpu(i)
    refcnt += *per_cpu_ptr(dev->pcpu_refcnt, i);

// per_cpu_ptr 使用例子2 写一个变量 
// net/xfrm/xfrm_ipcomp.c::ipcomp_decompress()
const int cpu = get_cpu();
void *ptr_r = *per_cpu_ptr(per_cpu_var, cpu);
/* ... access via ptr_r */
put_cpu();

```

# 8. bootmem 启动时申请内存
1. 申请的代码必须linkin内核中; 不允许module调用
2. 申请的内存bypass所有的内存管理机制; 使得运行后无法再被复用
## 8.1 接口
```c++
#define alloc_bootmem(x)           // 默认申请 high-memory段
#define alloc_bootmem_low(x)       // 指定申请 low-memory段
#define alloc_bootmem_pages(x)     // 申请pages
#define alloc_bootmem_low_pages(x) // 申请low-memory段的pages

// 由于bypass内存管理机制; 释放时必须传入申请时size
void free_bootmem(unsigned long physaddr, unsigned long size);
```
