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

场景: 解决了什么问题?
原理 or 机制: 宏观的框架?
方法: 有哪些接口可以调用? 怎么调用的? 哪里有例子?
实现 ldd3中这个概念比较弱


# Network interfaces tips
Network interfaces are the third standard class of Linux devices. (1. char; 2. block)

not in /dev/
not file_operations
Several hundred sockets can be multiplexed on the same physical interface.

块设备   只需要 响应 内核的请求
网络设备 还需要 响应 收包; 然后回传到协议栈
一系列 administrative tasks:
 setting addresses, modifying transmission parameters,
 maintaining  traffic  and  error  statistics

The  network  subsystem  of  the  Linux  kernel  is  designed  to  be  completely  protocol-independent.