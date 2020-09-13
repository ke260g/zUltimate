[TOC]
+ 临界段: 读写共享数据的代码段
+ 原子化: 一系列的指令要么不开始; 开始后到执行结束前不可被打断; 即临界区不可分割
+ 竞争条件: 多个线程同时在同一 临界区 读写共享数据
+ 锁的区别: 当锁不可用时线程的表现形式(阻塞 / 返回 / 重来)

锁的最底层就是原子变量: x86 下 compare  和 exchange  指令

+ 占用的类型 = 阻塞 / 非阻塞 / 超时 / 阻塞可唤醒
+ 中断里面 必须用spinlock
+ spinlock 是锁的底层本质, 其他类型锁都是在spinlock的基础上封装"业务"与"休眠"

# 1 3 rules and 2 requirements
1. design your driver is to avoid shared resources whenever possible. 
2. In SMP, any time that a hardware or software resource is shared beyond a single thread of execution, and the possibility exists that one thread could encounter an inconsistent view of that resource, you must explicitly manage access to that resource.
3. In SMP, when kernel codecreates an object that will be shared with any other part of the kernel, that objectmust continue to exist (and function properly) until it is known that no outside refer-ences to it exist. 满足该rule的要求:
    1. no objectcan be made available to the kernel until it is in a state where it can function properly
    2. references to such objects must be tracked

# 内核中竞争产生的原因
1. 中断; 硬中断产生回调
2. softirq 和 tasklet 内核任何时刻都可以调度
3. 抢占; 内核 task_struct 间可以互相抢占
4. 休眠; 内核 task_struct 进入休眠态 导致用户进程执行
5.  smp: 多处理器架构真正意义上并行执行代码

# 竞争设计checklist
1. 数据是不是全局的; 是否多线程可以访问
2. 数据是否在  进程上下文 和 中断上下文中共享; 是否在多个中断回调中共享
3. 数据被进程访问时 是否可能被抢占; 被调度的task_struct 是否访问同一数据
4. 数据在被进程占用时; 进程是否会休眠 or 阻塞
5. 函数被多个处理器同时执行; 是否保证互斥

# 死锁产生条件
1. 线程自己死锁; 重新获取已占用但没有释放的锁
2. 多线程 多个锁获取顺序相反


# 可休眠锁 _killable 和 _interruptible 的区别
1. 跟踪 xxx_killabel xxx_interruptible 的调用栈
2. killable 的 state 参数 TASK_WAKEKILL, 只有受到kill信号才返回
3. interruptible 的 state 参数 TASK_INTERRUPTIBLE, 任意信号可返回
```c++
int signal_pending_state(long state, struct task_struct *p)
{
    if (!(state & (TASK_INTERRUPTIBLE | TASK_WAKEKILL)))
        return 0;
    if (!signal_pending(p))
        return 0;

    return (state & TASK_INTERRUPTIBLE) || __fatal_signal_pending(p);
}
```

# 历史问题
## 2.1 aba problem
## 2.2 write starvation

# 实现依赖
```txt
atomic  > spinlock > {  completion / semphore / mutex / rwlock }
seqlock
rcu
barrier > kfifo
```