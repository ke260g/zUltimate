[TOC]
# 1. 内存屏障
## 1.1 barrier() （compiled barrier)
```c++
# define barrier() __memory_barrier()

```
内存屏障: 编译器层的控制
  Compiled code stores to memory all values that are currentlymodified and resident in CPU registers,
  and rereads them later when they areneeded.
  A call tobarrierprevents compiler optimizations across the barrier butleaves the hardware free to do its own reordering.
+ 与硬件完全无关
+ 内存屏障的本质之一 = 停止 编译优化/CPU流水线; 避免 优化导致硬件读写的时序问题 

### 1.2 mb() / rmb() / wmb() / read_barrier_depends()（hardware barrier)
```c++
// friendly macro
#define set_mb(var, value)  do { (var) = (value); mb(); } while (0)
```
1. 代码中包含 barrier(); 且通过调用arch指令; 实现硬件级的 内存屏障
2. rmb()  读屏障
3. wmb()  写屏障
4. mb() 读写屏障

### 1.3 smp() (smp hardware barrier)
```c++
void smp_rmb(void);
void smp_read_barrier_depends(void);
void smp_wmb(void);
void smp_mb(void);
```

# 2. ioports
```c++
struct resource *__request_region(struct resource *parent,
				   resource_size_t start, resource_size_t n,
				   const char *name, int flags);
void __release_region(struct resource *parent, resource_size_t start,
			resource_size_t n);
```
## 2.1 方法 (申请 / 释放 / 读写 / 读写流 )
```c++
// 申请 / 释放
#define request_region(start,n,name)		__request_region(&ioport_resource, (start), (n), (name), 0)
#define release_region(start,n)	__release_region(&ioport_resource, (start), (n))

// 读写
u8 inb(unsigned long addr);
u16 inw(unsigned long addr);
u32 inl(unsigned long addr);

void outb(u8 b, unsigned long addr);
void outw(u16 b, unsigned long addr);
void outl(u32 b, unsigned long addr);

// 读写 流
void insb(unsigned port, void *addr, unsigned long count);
void insw(unsigned port, void *addr, unsigned long count);
void insl(unsigned port, void *addr, unsigned long count);

void outsb(unsigned port, void *addr, unsigned long count);
void outsw(unsigned port, void *addr, unsigned long count);
void outsl(unsigned port, void *addr, unsigned long count);
```

## 2.2 调试
/proc/ioports


# 3. iomemory
## 3.1 接口 (创建 / 释放 / 映射 / 反映射 / 读写 / 读写repeat / 读写block)
```c++
// 创建 / 释放
#define request_mem_region(start,n,name) __request_region(&iomem_resource, (start), (n), (name), 0)
#define release_mem_region(start,n)	__release_region(&iomem_resource, (start), (n))

// 映射 / 反映射
void *ioremap(phys_addr_t offset, unsigned long size);
void *ioremap_nocache(phys_addr_t offset, unsigned long size);
void iounmap(void * addr);

// 读写
unsigned int ioread8(void __iomem *addr)
unsigned int ioread16(void __iomem *addr)
unsigned int ioread32(void __iomem *addr)

void iowrite16(u16 val, void __iomem *addr);
void iowrite16(u16 val, void __iomem *addr);
void iowrite32(u32 val, void __iomem *addr);

// 读写 repeat
void ioread8_rep(void __iomem *addr, void *dst, unsigned long count);
void ioread16_rep(void __iomem *addr, void *dst, unsigned long count);
void ioread32_rep(void __iomem *addr, void *dst, unsigned long count);

void iowrite8_rep(void __iomem *addr, const void *src, unsigned long count);
void iowrite16_rep(void __iomem *addr, const void *src, unsigned long count);
void iowrite32_rep(void __iomem *addr, const void *src, unsigned long count);

// 读写 iomem > memory
void memset_io(void *addr, u8 value, unsigned int count);
void memcpy_fromio(void *dest, void *source, unsigned int count);
void memcpy_toio(void *dest, void *source, unsigned int count);
```

## 3.2 调试
/proc/iomem