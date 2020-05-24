[TOC]
各种锁具体原理这里不细说  销毁方法不赘述
+ 锁的本质就是  占用 + 释放 (compelition 概念相思)
+ 占用的类型 = 阻塞 / 非阻塞 / 超时 / 阻塞可唤醒
+ 中断里面 必须用spinlock
+ spinlock 的实现相当负责; 与CPU架构强相关, 也是锁的底层本质, 其他类型锁都是在spinlock的基础上封装"业务"与"休眠"
# 1 3 rules and 2 requirements
1. design your driver is to avoid shared resources whenever possible. 
2. In SMP, any time that a hardware or software resource is shared beyond a single thread of execution, and the possibility exists that one thread could encounter an inconsistent view of that resource, you must explicitly manage access to that resource.
3. In SMP, when kernel codecreates an object that will be shared with any other part of the kernel, that objectmust continue to exist (and function properly) until it is known that no outside refer-ences to it exist. 满足该rule的要求:
    1. no objectcan be made available to the kernel until it is in a state where it can function properly
    2. references to such objects must be tracked

# 2 semaphore.h ( 创建 / 占用 / 释放 )
```c++
#define DEFINE_SEMAPHORE(name)  // 信号值为1
void sema_init(struct semaphore *sem, int val);

//  占用  
void down(struct semaphore *sem); // 阻塞在内核态 出不来
int down_trylock(struct semaphore *sem);
int down_timeout(struct semaphore *sem, long jiffies);
int down_killable(struct semaphore *sem); // 阻塞, 但只有SIGKILL才能唤醒
int down_interruptible(struct semaphore *sem); // 一般用这个, 阻塞且可中断, 上层根据retval 处理异常逻辑

// 释放
extern void up(struct semaphore *sem); 
```

# 3 rwsem.h ( 创建 / r占用 / w占用 / r释放 / w释放 )
```c++
#define DECLARE_RWSEM(name)
#define init_rwsem(sem)

// r占用
void down_read(struct rw_semaphore *sem);
int down_read_trylock(struct rw_semaphore *sem);

// w占用
void down_write(struct rw_semaphore *sem);
int down_write_trylock(struct rw_semaphore *sem);

// r释放
up_read(struct rw_semaphore *sem);

// w释放
up_write(struct rw_semaphore *sem);

// 把 w占用 转换为 r占用
void downgrade_write(struct rw_semaphore *sem);
```
## 3.1 downgrade_write
1. 原文是说 当一个写保护后，跟着一个耗费时间长的读保护 才适用 downgrade_write
+ 本质 = 原子化  w占用 切换 r占用
+ 目的 = 避免其他线程消耗更多的 r占用时间

# 4 completion ( 创建 /  等待 / 通知 )
```c++
void init_completion(struct completion *x);

// 等待 ( 阻塞 / 非阻塞 / 超时 / 阻塞可唤醒 )
void wait_for_completion(struct completion *); // 阻塞
bool try_wait_for_completion(struct completion *x); // 非阻塞
unsigned long wait_for_completion_timeout(struct completion *x,unsigned long timeout);
int wait_for_completion_interruptible(struct completion *x); // 阻塞但可唤醒 一般用这个
int wait_for_completion_killable(struct completion *x);

// IO等待
void wait_for_completion_io(struct completion *x); // 不可被唤醒, 通常是IO等待

// 通知
void complete(struct completion *c);
void complete_all(struct completion *c);

// 检查有没有 等待者
bool completion_done(struct completion *x); 
```

## 4.1 本质
1. sem 进行了性能优化 专门处理 (大部分时间是 可行, 少部分时间是 不行)
    1. 优化点1: likely
    2. 优化点2: 流程
2. 但是 存在在大部分 时间是不行的场景 比如初始化
+ `down()` 开始调用了 `likely()`
+ `wait_for_completion()` 没有使用 `likely()`

# 5 spinlock (创建 / 占用 / 释放)
```c++
// 创建
spinlock_t lock;
#define spin_lock_init(_lock)

// 占用
void spin_lock(spinlock_t *lock);
#define spin_lock_irqsave(lock, flags) // 保存中断上下文 停止其他中断
void spin_lock_irq(spinlock_t *lock);  // 忽略中断上下文 停止其他中断
void spin_lock_bh(spinlock_t *lock);   // 忽略中断上下文 仅停止软中 允许硬中断 (参考 中断机制)


// 释放
void spin_unlock(spinlock_t *lock);
void spin_unlock_irqrestore(spinlock_t *lock, unsigned long flags);void spin_unlock_irq(spinlock_t *lock);
void spin_unlock_bh(spinlock_t *lock);

// 非阻塞性占用; 返回非零 表示成功占用; 成功占用后需要释放
int spin_trylock(spinlock_t *lock);
#define spin_trylock_irqsave(lock, flags)
int spin_trylock_bh(spinlock_t *lock);
int spin_trylock_irq(spinlock_t *lock);

// 检查
int spin_is_locked(spinlock_t *lock);
```
1. 自旋锁是最底层的锁(cpu 直接while 1) uninterruptible 的
2. 实现要素 = 

## 5.1 自选锁以及原子上下文的 场景
### 5.1.1 spinlock 占用前停止其他中断 (实现)
硬件中断回调(interrupt_handle) 不允许休眠，一般都使用spinlock
如果在 spinlock 被占用时, 产生了硬件中断回调, 而回调中占用spinlock 就会导致死锁
+ 因此 spinlock 占用前停止其他中断
+ 因此 硬件中断可能会丢掉
### 5.1.2 spinlock 占用时间尽可能短 (使用)
由于spinlock 占用前会停止其他中断 可能导致中断丢失
如果占用时间长 将提高中断丢失的概率
所以 spinlock 占用时间尽可能短
### 5.1.3 spinlock 占用后不能休眠 (使用)
spinlock 占用后如果休眠, 则CPU将在足够长的时间后才释放
而如此足够长的时间导致其他 代码无法响应，
其他代码可能也会尝试占用，导致CPU死锁

# 99 killable 和 interruptible 的区别
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