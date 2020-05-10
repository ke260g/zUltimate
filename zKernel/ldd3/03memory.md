kmalloc looks rather different from a typical user-spacemalloc implementation. 
A simple, heap-oriented allocation technique would quickly run into trouble;
it would have a hard time working around the page boundaries.

Linux handles memory allocation by creating a set of pools of memory objects offixed sizes. 
Allocation requests are handled by going to a pool that holds sufficiently large objects and 
handing an entire memory chunk back to the requester. 

The kernel canallocate  only certain predefined,  fixed-size byte  arrays.
The  smallest  allocation  thatkmalloccan handle is as big as 32 or 64 bytes, depending on the page size used bythe system’s architecture.

# kmalloc 
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
| GFP_ATOMIC | Never Sleep | interrupt; outside of aprocess context |
| GFP_KERNEL | May Sleep   | Most normal allocation |
| GFP_USER   | May Sleep   | allocates user-space pages |

# slab （memory pool) /proc/slabinfo
```c++
struct kmem_cache *kmem_cache_create(const char *name, 
            size_t size, size_t align,
            unsigned long flags, void (*ctor)(void *));
void kmem_cache_destroy(struct kmem_cache *s);
int kmem_cache_shrink(struct kmem_cache *cachep);

void *kmem_cache_alloc(struct kmem_cache *, gfp_t flags);
void kmem_cache_free(struct kmem_cache *c, void *b)
```
1. The cache keeps a pointer to the name, rather than copying it, 
so the driver should pass in apointer to a name in static storage (usually the name is just a literal string). 
2. The name cannot contain blanks
3. each cache can be traced in /proc/slabinfo

# mempool (interface wrapper of slab)
```c++
mempool_t *mempool_create(int min_nr, mempool_alloc_t *alloc_fn,
				mempool_free_t *free_fn, void *pool_data);
int mempool_resize(mempool_t *pool, int new_min_nr, gfp_t gfp_mask);
void mempool_destroy(mempool_t *pool);

extern void *mempool_alloc(mempool_t *pool, gfp_t gfp_mask); // shadowly call alloc_fn
extern void mempool_free(void *element, mempool_t *pool);    // shadowly call free_fn
```

# get_zeroed_page (big chucks of memory)
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

void __free_pages(struct page *page, unsigned int order);
void free_pages(unsigned long addr, unsigned int order);
#define __free_page(page) __free_pages((page), 0)
#define free_page(addr) free_pages((addr), 0)

// the strange numeric representation of `order'(2rd-argu of __get_free_pages)
// is designed for memsetting with bit-operation.
void * alloc_pages(int order) {
    void *p = (void *)__get_free_pages(GFP_KERNEL, order);
    if (!p)
        goto error;
    memset(p, 0, PAGE_SIZE << order);
error:
    return p;
}
```

# vmalloc 尽量少用
+ 性能不好
```c++
void *vmalloc(unsigned long size);
void vfree(const void *addr);
void __iomem *ioremap(phys_addr_t offset, unsigned long size);
void iounmap(void __iomem *addr)
```