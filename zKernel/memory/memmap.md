basic unit: 4K

All CPU access (including from kernel space) uses virtual addresses that are translated by the MMU into physical addresses with the help of page tables

A physical page of memory is identified by the Page Frame Number (PFN). 

For efficiency reasons, the virtual address space is divided into user space and kernel space.

For efficiency reasons,  the kernel space contains a memory mapped zone, called lowmem, which is contiguously mapped in physical memory, starting from the lowest possible physical address (usually 0). The virtual address where lowmem is mapped is defined by PAGE_OFFSET.

+ lowmem: which is contiguously mapped in physical memory, starting from the lowest possible physical address (usually 0). The virtual address where lowmem is mapped is defined by PAGE_OFFSET
+ highmem:
a separate zone in kernel space called highmem which can be used to arbitrarily map physical memory

# Structure
## 1. struct page
embed information about all physical pages
`struct page` > page-frame-number > virtual-address >
### 1.1 member 
### 1.2 method
virt_to_page()
pfn_to_page()
page_to_pfn()
page_address() 
kmap()
## 2. struct vm_area_struct
holds information about a contiguous virtual memory area
cat /proc/1/maps
+ a page =  a start address, a stop address, length, permissions.
A struct vm_area_struct is created at each mmap() call issued from user space. A driver that supports the mmap() operation must complete and initialize the associated struct vm_area_struct. The most important fields of this structure are:
### 2.1 member
+ vm_start, vm_end (/proc/$pid/maps);
+ vm_file - the pointer to the associated file structure (if any);
+ vm_pgoff - the offset of the area within the file;
+ vm_flags - a set of flags;
+ vm_ops - a set of working functions for this area
+ vm_next, vm_prev
### 2.2 method


## 3. struct mm_struct
### 3.1 member
+ mm: pointed to a process
### 3.2 method

# kernel and user share a memory
## Device driver memory mapping
1. 系统调用 `void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);`
    + 系统调用通过 open获取设备fd对象, mmap的fd参数指示设备fd, 从而实现 内核空间到用户空间的映射
2. `struct file_operations { int (*mmap) (struct file *, struct vm_area_struct *);  };`
    1. 设备驱动, 实现这个方法
    2. 设备驱动, 被内核空间的内存, 通过vm_area_struct对象返回
3. `int remap_pfn_range (struct vm_area_struct *vma, unsigned long addr, unsigned long pfn, unsigned long size, pgprot_t prot);`
    1. 设备驱动, 调用kmalloc(), vmalloc(), alloc_pages(), 申请内存
    2. 设备驱动, 调用remap_pfn_range, 传入申到的内存, 传出 vm_area_struct
```c++
// 1.1 申请内存::直接映射法 
static char *kmalloc_area;
unsigned long pfn = virt_to_phys((void *)kmalloc_area)>>PAGE_SHIFT;
// 1.2 申请内存::vmalloc()
static char *vmalloc_area;
unsigned long pfn = vmalloc_to_pfn(vmalloc_area);
// 1.3 申请内存::alloc_pages()
struct page *page;
unsigned long pfn = page_to_pfn(page);
// 2. remap_pfn_range用法
struct vm_area_struct *vma = kmalloc();
unsigned long len = vma->vm_end - vma->vm_start;
remap_pfn_range(vma, vma->vm_start, pfn, len, vma->vm_page_prot);
```

## avoid swapped out
+ alloc逻辑中 调用 SetPageReserved
```c++
struct page *page;
for (page = virt_to_page(start_addr); page <= virt_to_page(end_addr); page++)
    SetPageReserved(page);
```
+ free逻辑中  调用 ClearPageReserved
```c++
struct page *page;
for (page = virt_to_page(start_addr); page <= virt_to_page(end_addr); page++)
    ClearPageReserved(page);
```

## handle not contiguous issues of vmalloc()
ave to map each page individually and compute the physical address for each page

