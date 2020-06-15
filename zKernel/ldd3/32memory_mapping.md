[TOC]
# 1. 内存管理特性摘要
## 1.1 地址类型
user virtual    = 用户态的虚拟地址
kernel virtual  = 内核态的虚拟地址; high-memory; 不连续; 通常是vmalloc申请的 
physical        = 处理器 + 内存条的物理地址
kernel logical  = 内核态地址; low-memory; 一整块线性映射到硬件中; 内核主要代码运行于此
bus             = 在内核中形同物理地址; 但硬件上连接着bus; 用于bus硬件读写
## 1.2  struct page
### 1.2.1 定义
+ 此处只记录关键的、用于驱动开发的 接口和成员变量; 
```c++
struct page {
    atomic_t count; // 引用计数; == 0 则释放
    void *virtual;  // kernel virtual address 内核虚拟地址; NULL表示没有经过映射(kmap)
    unsigned long flags; //  page 的状态标记: 如atomic 
}
```
### 1.2.2 方法 （pfn / page / virt 三者间转换)
+ 一般对于开发者; phys与page的转换并不常用; 有些架构甚至没有对外接口
+ page 后面是phys地址, 由架构底层代码玩过; 没有page与phys的转换接口
+ 使用 `ioremap_*()` 接口 访问phys地址
```c++
// pfn <-> virt <-> page
//  ^-----------------^
#define page_to_virt(page)	
#define virt_to_page(addr)	// 然会 logical address的 page （注意是内核logical地址; 不是内核virtual地址)

#define virt_to_pfn(kaddr)
#define pfn_to_virt(pfn)

#define page_to_pfn(page)
#define pfn_to_page(pfn)

/** @note 对于low-mem没有mapping的只有offset
 *        对应high-mem 进行mapping后需要 kunmap */
void *kmap(struct page *page);   // 返回pages的virtual地址; low-mem即logical address; high-mem则建立映射
void kunmap(struct page *page);  // 释放high-mem的映射

// 非阻塞性kmap
void *kmap_atomic(struct page *page, enum km_type type);
void kunmap_atomic(void *addr, enum km_type type);
```

## 1.3 struct vm_area_struct
### 1.3.1 定义
```c++
struct vm_area_struct {
    unsigned long vm_start; // virtual memory area start; /proc/$pid/maps 第一列
    unsigned long vm_end;   // virtual memory area end;   /proc/$pid/maps 第二列
    pgprot_t vm_page_prot;  // access permission;         /proc/$pid/maps 第三列
}
```

## 1.4 struct mm_struct
### 1.4.1 定义
1. task_struct 表示一个进程;
2. 每个进程 mm_struct 记录进程的内存分布
3. 其中 mm_struct->mmap 成员记录了一系列的 vm_area_struct
```c++
struct mm_struct {
    struct vm_area_struct *mmap; /* list of VMAs */
    spinlock_t page_table_lock;  /* Protects page tables and some counters */ /* 例如: 驱动改写用户进程的映射 */
};
struct task_struct {
    struct mm_struct *mm;
};
```

# 2. 内存映射 page table
## 驱动实现mmap机制
1. 用户态系统调用 mmap
2. 内核封装了的 mmap系统调用到 file_operations->mmap 的逻辑
    + `mmap (caddr_t addr, size_t len, int prot, int flags, int fd, off_t offset);`
3. 因此, 设备驱动只需要实现 file_operations->mmap
    + `int (*mmap) (struct file *filp, struct vm_area_struct *vma);`
## 实现 file_operations->mmap
1. vma是传入参数; 对应与 进程 task_struct->mm_struct->vm_area_struct 的一个节点
2. vma->start, vma->end 是内核封装好的
3. 因此, 驱动需要把vma映射到设备的物理地址
### 方案1 映射 vma->start 到设备地址
+ `drivers/char/hpet.c`
+ `drivers/char/bsr.c`
### 方案2 重写 vma->vm_ops
+ `drivers/char/agp/frontend.c`
+ `drivers/char/mspec.c`