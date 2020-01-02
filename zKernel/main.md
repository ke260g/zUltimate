[TOC]
first class
https://blog.csdn.net/stone8761/article/category/6858049

jiffies

内核驱动Makefile
```Makefile
CONFIG_MODULE_SIG=n # 避免签名问题 
obj-m:=hello.o
hello-objs:=helloworld.o
 
KERNEL_ROOT=/lib/modules/`uname -r`/build/
CUR_DIR=$(shell pwd

default:
        $(MAKE) -C $(KERNEL_ROOT) M=$(CUR_DIR) modules
 
clean:
        rm -rf *.o *.ko *.mod.c .*.cmd .*.cmd.c modules.* Module.* .tmp_versions
```

分配内存
static __always_inline void *kmalloc(size_t size, gfp_t flags);
void kfree(const void *);
最常用的flags:
GFP_ATOMIC, 从不睡眠, 用来从中断处理和进程上下文之外的其他代码中分配
GFP_KERNEL, 可能休眠, 一般性分配, 
GFP_USER  , 可能睡眠, 用来为用户空间页来分配内存

自旋锁
DEFINE_SPINLOCK(x)
spin_lock_init(_lock);
static inline void spin_lock(spinlock_t *lock);
static inline void spin_unlock(spinlock_t *lock);

信号量
static inline void sema_init(struct semaphore *sem, int val);
extern void down(struct semaphore *sem); 不可中断的down，进程如果获取不到资源, 则无法中断和kill
extern int __must_check down_killable(struct semaphore *sem); 可中断的down, 被fatal信号中断
extern int __must_check down_trylock(struct semaphore *sem);  非阻塞性
extern int __must_check down_timeout(struct semaphore *sem, long jiffies); 定时返回
extern void up(struct semaphore *sem);

阻塞性延时
#include <linux/delay.h>
void ndelay(unsigned long nsecs);
void udelay(unsigned long usecs);
void mdelay(unsigned long msecs);

非阻塞性延时
void msleep(unsigned int millisecs);
void ssleep(unsigned int seconds);
unsigned long msleep_interruptible(unsigned int millisecs);
让进程进入可中断的睡眠状态, 返回被提早唤醒时剩余的毫秒数

定时器, 回调在软中断上下文中
void init_timer(struct timer_list *timer);
int add_timer(struct timer_list *timer); // 添加一个定时器到内核动态定时器链表
int del_timer(struct timer_list *timer);
int del_timer_sync(struct timer_list *timer);
int mod_timer(struct timer_list *timer); // 修改已经在内核定时器链表中的定时器
不能进行睡眠或者睡眠调度.
原子代码不能调用 schedule 或者某种 wait_event,
也不能调用任何其他可能睡眠的函数. 如kmalloc(..., GFP_KERNEL)
应该用 kmalloc(..., GFP_ATOMIC)

与用户层通讯手段: netlink, /proc, ioctl回调, sockopt, 共享内存

