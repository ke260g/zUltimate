[TOC]
# 1. 内存管理特性摘要
## 1.1 地址类型
user virtual    = 用户态的虚拟地址
kernel virtual  = 内核态的虚拟地址; high-memory; 不连续; 通常是vmalloc申请的 
physical        = 处理器 + 内存条的物理地址
kernel logical  = 内核态地址; low-memory; 一整块线性映射到硬件中; 内核主要代码运行于此
bus             = 在内核中形同物理地址; 但硬件上连接着bus; 用于bus硬件读写
## 1.2  page
### 1.2.1 定义
+ 此处只记录关键的、用于驱动开发的 接口和成员变量; 
```c++
struct page {
    atomic_t count; // 引用计数; == 0 则释放
    void *virtual;  // kernel virtual address 内核虚拟地址; NULL表示没有经过映射(kmap)
    unsigned long flags; //  page 的状态标记: 如atomic 
}

void put_page(struct page *page); // 释放索引 count--
void get_page(struct page *page); // 占用索引 count++
```
### 1.2.2 方法 （pfn / page / virt 三者间转换)
+ 一般对于开发者; phys与page的转换并不常用; 有些架构甚至没有对外接口
+ page 后面是phys地址, 由架构底层代码玩过; 没有page与phys的转换接口
+ 使用 `ioremap_*()` 接口 访问phys地址
```c++
// pfn / virtal / page 三者对象的转换
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

## 1.3 vm_area_struct / vm_operations_struct
### 1.3.1 定义
```c++
/**
 * @open:  // vm有新的进程引用的回调; 即进程fork;
 * @close: // 进程退出时关闭vm的回调; 即进程exit;
 */
struct vm_operations_struct {
    void (*open)(struct vm_area_struct * area);  
	void (*close)(struct vm_area_struct * area);
    // ...
};

struct vm_area_struct {
    unsigned long vm_start; // virtual memory area start; /proc/$pid/maps 第一列
    unsigned long vm_end;   // virtual memory area end;   /proc/$pid/maps 第二列
    pgprot_t vm_page_prot;  // access permission;         /proc/$pid/maps 第三列
    struct vm_operations_struct *vm_ops;
    void *vm_private_data;
};
```

## 1.4 mm_struct
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

# 2. 驱动实现实现内存映射
+ 场景 = 网卡设备; 视频设备
## 实现 mmap 机制
1. 用户态系统调用 mmap; 传入参数 用户态内存地址; 传入参数 fd指向设备;
2. 内核封装了的 mmap系统调用到 file_operations->mmap 的逻辑 (参数 fd 指向设备)
    + `mmap (caddr_t addr, size_t len, int prot, int flags, int fd, off_t offset);`
    + 封装内容 = 申请并初始化 vma_area_struct; 加入到 mm_struct;
3. 因此, 设备驱动只需要实现 file_operations->mmap
    + `int (*mmap) (struct file *filp, struct vm_area_struct *vma);`
    + 参数 `vma` 指示用户态的内存页(proc/$pid/maps的一项) 

## 实现 mmap 步骤 ( remap_pfn_range + 重载 vma->vm_ops )
1. vma是传入参数; 对应与 进程 task_struct->mm_struct->vm_area_struct 的一个节点
2. vma->start, vma->end 是内核封装好的
3. 因此, 驱动需要把vma映射到设备的物理地址
4. remap_pfn_range(传入vm->start, 设备硬件地址);
5. 必要时, 重载 vma->vm_ops
### remap_pfn_range 实现限制
1. 只能使用 物理地址的高地址段 和 保留page

## 实现 mmap 示例 
+ `drivers/char/hpet.c`
+ `drivers/char/bsr.c`
+ `drivers/char/mmtimer.c`
+ `drivers/char/agp/frontend.c` (实现vma->vm_ops)
+ `drivers/char/mspec.c`        (实现vma->vm_ops)
```c++
/**
 * remap_pfn_range - remap kernel memory to userspace
 * @vma: user vma to map to
 * @addr: target user address to start at
 * @pfn: physical address of kernel memory; 该参数一般是 phys_addr >> PAGE_SIZE
 * @size: size of map area
 * @prot: page protection flags for this mapping
 *
 *  Note: this is only safe if the mm semaphore is held when called.
 */
int remap_pfn_range(struct vm_area_struct *vma, unsigned long addr,
		    unsigned long pfn, unsigned long size, pgprot_t prot);

// 使用例子; 设备驱动实际上只需要构建 phys_addr
int device_mmap(struct file *file, struct vm_area_struct *vma)
{
    unsigned long phys_addr;
    device_get_phys_addr(&phys_addr);
    
    // is_ram_memory is_io_memory 是伪代码
    if (is_ram_memory(phys_addr)) {       // 如果物理地址是 在RAM中
        remap_pfn_range(vma, vma->vm_start, phys_addr >> PAGE_SHIFT, PAGE_SIZE, vma->vm_page_prot);
    } else if (is_io_memory(phys_addr)) { // 如果物理地址是 在IO memory中
        io_remap_pfn_range(vma, vma->vm_start, phys_addr >> PAGE_SHIFT, PAGE_SIZE, vma->vm_page_prot);
    }
    return 0;
}
```

# 3. 与用户态 I/O 交互
## 3.1 Direct I/0 (zero-copy)
### 3.1.1 接口
```c++
/**
 * get_user_pages() - pin user pages in memory                          <-- 获取用户进程 page
 * @tsk:	the task_struct to use for page fault accounting, or
 *		NULL if faults are not to be recorded.
 * @mm:		mm_struct of target mm
 * @start:	starting user address
 * @nr_pages:	number of pages from start to pin                       <-- 指示传出数组的长度
 * @write:	whether pages will be written to by the caller
 * @force:	whether to force access even when user mapping is currently
 *		protected (but never forces write access to shared mapping).
 * @pages:	array that receives pointers to the pages pinned.           <-- 传出参数
 *		Should be at least nr_pages long. Or NULL, if caller
 *		only intends to ensure the pages are faulted in.
 * @vmas:	array of pointers to vmas corresponding to each page.       <-- 传出参数
 *		Or NULL if the caller does not require them.
 */
long get_user_pages(struct task_struct *tsk, struct mm_struct *mm,
	    unsigned long start, unsigned long nr_pages, int write,
		int force, struct page **pages, struct vm_area_struct **vmas);
```
### 3.1.2 使用
```c++
void driver_routine() {
    down_read(&current->mm->mmap_sem); // 占用 进程vma列表 锁
    user_pages = get_user_pages(current, current->mm, ...);

    // 执行了写操作需要额外的
    if (write_to_user) {
        if (! PageReserved(page)) // 保留page 不会写入 swap分区
            SetPageDirty(page);   // 指示内核 该page 需要重新更新到 swap分区中(如果swap分区中存在)
        page_cache_release(page);
    }

    up_read(&current->mm->mmap_sem);   // 释放 进程vma列表 锁
}
``` 

## 3.2 Asynchronous I/O






