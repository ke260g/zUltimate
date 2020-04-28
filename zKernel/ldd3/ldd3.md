1. how to write your own drivers
2. how to hack around inrelated parts of the kernel


a device driver is providing mechanism, not policy.

Most programming problems can be splited into two parts:
1. what capabilities are to be provided(mechanism)
2. how capabilities can be used (policy)



Linux driver developers should pay particular attension to this fundamental concept: write kernel code to access the hardware, but don't force particular policies on the user.

trade-off between the desire to present the user with as many options as possible and the time you have to write the driver

Being policy-free is actually a common target for softwaredesigners.

Kernel Task:
process management
memory  management
filesystems
device control
networking


When reading this book, I gonna to understand what tools or resources could be used.
But not how to use it.

1. 不要记录头文件; 关键是知道 mechanism
2. 每一个书中提到到接口; 在内核源码中找一下

2 struct file_operations; struct file; struct inode;


printk
void *kmalloc(size_t size, int flags);  void kfree(void *ptr); 

这两个函数会休眠; 
因为 用户态地址 可能还没在 虚拟内存中, 映射虚拟内存是会休眠的
因为 内核态地址 可能还没在 虚拟内存中, 映射虚拟内存是会休眠的
```c++
unsigned long copy_to_user(void __user *to, const void *from, unsigned long count);
unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);
```







# /proc include/linux/proc_fs.h
+ can be used for communication between user
1. /proc dir  opertation `proc_symlink(); proc_mkdir(); remove_proc_subtree();`
2. /proc file opertation `proc_create(); proc_remove();`
3. /proc file_operations `struct proc_dir_entry->file_operations`

# include/linux/seq_file.h
+ 用于减少 /proc文件的体积
1. 实现 seq_operations
    + start/stop/next/show
2. 实现 file_operations 时 使用seq接口
    + open/release/read/write/llseek => seq_read/seq_release/seq_read/seq_write/seq_lseek
+ 最终流程 syscall -> file_operation -> seq_接口 -> seq_operations
+ 例子: uptime.c