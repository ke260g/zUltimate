[TOC]
各种锁具体原理这里不细说  销毁方法不赘述
+ 锁的本质就是  占用 + 释放 (compelition 概念相似)
+ 占用的类型 = 阻塞 / 非阻塞 / 超时 / 阻塞可唤醒
+ 中断里面 必须用spinlock
+ spinlock 是锁的底层本质, 其他类型锁都是在spinlock的基础上封装"业务"与"休眠"

# 1 3 rules and 2 requirements
1. design your driver is to avoid shared resources whenever possible. 
2. In SMP, any time that a hardware or software resource is shared beyond a single thread of execution, and the possibility exists that one thread could encounter an inconsistent view of that resource, you must explicitly manage access to that resource.
3. In SMP, when kernel codecreates an object that will be shared with any other part of the kernel, that objectmust continue to exist (and function properly) until it is known that no outside refer-ences to it exist. 满足该rule的要求:
    1. no objectcan be made available to the kernel until it is in a state where it can function properly
    2. references to such objects must be tracked

# 2 semaphore.h 
## 2.1 方法 ( 创建 / 占用 / 释放 )
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
## 2.2 实现

# 3 rwsem.h
## 3.1 方法 ( 创建 / r占用 / w占用 / r释放 / w释放 )
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
### 3.1.1 downgrade_write
1. 原文是说 当一个写保护后，跟着一个耗费时间长的读保护 才适用 downgrade_write
+ 本质 = 原子化  w占用 切换 r占用
+ 目的 = 避免其他线程消耗更多的 r占用时间
## 3.2 实现

# 4 completion
1. sem 进行了性能优化 专门处理 (大部分时间是 可行, 少部分时间是 不行)
    1. 优化点1: likely
    2. 优化点2: 流程
2. 但是 存在在大部分 时间是不行的场景 比如初始化
+ `down()` 开始调用了 `likely()`
+ `wait_for_completion()` 没有使用 `likely()`

## 4.1 方法 ( 创建 /  等待 / 通知 )
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

## 4.2 实现


# 5 spinlock
## 5.1 方法 (创建 / 占用 / 释放)
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
## 5.2 实现
1. `__acquire()` 和`__release()`, 使用macro`__context__()`  配合parser工具, 保证编译时成对出现 `spin_lock()` `spin_unlock()`
2. 占用前关闭内核抢占`preempt_disable()` 释放后开启内核抢占 `preempt_enable()`
3. 单处理器 与 多处理器区别实现
```c++
// <linux/spinlock.h> 中
#if defined(CONFIG_SMP) || defined(CONFIG_DEBUG_SPINLOCK)
# include <linux/spinlock_api_smp.h>
#else
# include <linux/spinlock_api_up.h>
#endif
```
4. 单处理器`spin_lock`的实现 只有 `preempt_disable()`和 `preempt_enable()` 没有变量获取的逻辑, `trylock()` 只会返回`true`
5. 多处理器获取 锁的变量值 都是与架构相关的
```c++
// 调用链
// spin_lock                 // include/linux/spinlock.h
// raw_spin_lock             // include/linux/spinlock.h
// _raw_spin_lock            // include/linux/spinlock_api_smp.h
// __raw_spin_lock           // kernel/locking/spinlock.c
//     do_raw_spin_lock      // include/linux/spinlock.h
//         arch_spin_lock    // arch/$arch/include/spinlock*.h 由相应arch汇编实现
//     do_raw_spin_trylock   // include/linux/spinlock.h
//         arch_spin_trylock // return test_and_set_bit
static inline void __raw_spin_lock(raw_spinlock_t *lock) {
	preempt_disable();
	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
}
```

### 5.2.1 自旋锁流程概述
+ 单处理器:
    1. local_save_irq    [optional]
    2. preempt_disable
    3. 执行代码
    4. preempt_enable
    5. local_restore_irq [optional]
+ 多处理器
    1. local_save_irq    [optional]
    2. preempt_disable
    3. spin_acquire      [后续待研究]
    4. arch_spin_lock
    5. 执行代码
    6. spin_release      [后续待研究]
    7. do_raw_spin_unlock
    8. preempt_enable
    9. local_restore_irq [optional]

## 5.3 自选锁以及原子上下文的 场景
### 5.3.1 spinlock 占用前停止其他中断 (实现)
硬件中断回调(interrupt_handle) 不允许休眠，一般都使用spinlock
如果在 spinlock 被占用时, 产生了硬件中断回调, 而回调中占用spinlock 就会导致死锁
+ 因此 spinlock 占用前停止其他中断
+ 因此 硬件中断可能会丢掉
### 5.3.2 spinlock 占用时间尽可能短 (使用)
由于spinlock 占用前会停止其他中断 可能导致中断丢失
如果占用时间长 将提高中断丢失的概率
所以 spinlock 占用时间尽可能短
### 5.3.3 spinlock 占用后不能休眠 (使用)
spinlock 占用后如果休眠, 则CPU将在足够长的时间后才释放
而如此足够长的时间导致其他 代码无法响应，
其他代码可能也会尝试占用，导致CPU死锁
### 5.3.3 spinlock 多锁占用时 必须遵守同样的顺序占用
当存在资源需要多个 lock1 lock2 lockN 时
在不同的占用代码地方 都需要遵守同样的顺序占用

# 6 原子操作integer (arch相关) (高性能锁)
## 6.1 方法( 创建 / 读 / 写 )
```c++
atomic_t v = ATOMIC_INIT(0);

void atomic_set(atomic_t *v, int i);

// 原子写
int atomic_read(atomic_t *v);

void atomic_add(int i, atomic_t *v);
void atomic_sub(int i, atomic_t *v);
void atomic_inc(atomic_t *v);
void atomic_dec(atomic_t *v);

// 原子操作: 写+读, 仅返回 true/false
int atomic_inc_and_test(atomic_t *v);
int atomic_dec_and_test(atomic_t *v);
int atomic_sub_and_test(int i, atomic_t *v); // no atomic_add_and_test

// 原子操作: 写+读, 返回 具体的值
int atomic_add_return(int i, atomic_t *v);
int atomic_sub_return(int i, atomic_t *v);
int atomic_inc_return(atomic_t *v);
int atomic_dec_return(atomic_t *v);

// 原子操作: 写+读, 返回 具体的值的负数
int atomic_add_negative(int i, atomic_t *v);
```

# 6.2 实现

# 7. 原子操作bit (arch相关) (高性能锁)
## 7.1 方法 ( 设置/ 置位 / 清零 / 反相 / 读取 )
```c++
set_bit(bit, addr)			     // *addr |= bit
clear_bit(bit, addr)			 // *addr &= ~bit
change_bit(bit, addr)		     // *addr ^= bit
test_bit(bit, addr)			     // Is bit set in *addr?
test_and_set_bit(bit, addr)		 // Set bit and return old value
test_and_clear_bit(bit, addr)	 // Clear bit and return old value
test_and_change_bit(bit, addr)	 // Change bit and return old value
find_first_zero_bit(addr, nbits) //	Position first zero bit in *addr
find_first_bit(addr, nbits)		 // Position first set bit in *addr
find_next_zero_bit(addr, nbits, bit) // Position next zero bit in *addr >= bit
find_next_bit(addr, nbits, bit)	 // Position next set bit in *addr >= bit
```

## 7.2 实现

## 7.3 使用原子操作bit 替代锁; 以提高性能
```c++
/* try to set lock */
while (test_and_set_bit(nr, addr) != 0)
    wait_for_a_while( );

/* do your work */

/* release lock, and check... */
if (test_and_clear_bit(nr, addr) == 0)
    something_went_wrong( ); /* already released: error */
```
+ 书上这么写; 但实在没找到例子

# 8 seqlock 只用于一个integer (arch相关) (高性能锁)
1. 使用场景: 写比较罕见但很快; 但是读比较频繁的场景;
2. 使用逻辑: 非阻塞性读; 读完之后处理; 处理后检查是否有写更新;
3. 使用范式
```c++
 // Expected non-blocking reader usage:
do {
    seq = read_seqbegin(&value);
    //	...
} while (read_seqretry(&value, seq));
```
4. 使用例子 kernel/time/jiffies.c
```c++
u64 get_jiffies_64(void)
{
	unsigned long seq;
	u64 ret;

	do {
		seq = read_seqbegin(&jiffies_lock);
		ret = jiffies_64;
	} while (read_seqretry(&jiffies_lock, seq));
	return ret;
}
```

## 8.1 方法
```c++
#define seqlock_init(x) // 初始化

// r占用
unsigned read_seqbegin(const seqlock_t *sl);
unsigned int read_seqbegin_irqsave(seqlock_t *lock, unsigned long flags);
// r释放
int read_seqretry_irqrestore(seqlock_t *lock, unsigned int seq, unsigned long flags);
// r占用后检查是否需要重来
unsigned read_seqretry(const seqlock_t *sl, unsigned start);

// w占用
void write_seqlock(seqlock_t *lock);
void write_seqlock_irqsave(seqlock_t *lock, unsigned long flags);
void write_seqlock_irq(seqlock_t *lock);
void write_seqlock_bh(seqlock_t *lock);

// w释放
void write_sequnlock(seqlock_t *lock);
void write_sequnlock_irqrestore(seqlock_t *lock, unsigned long flags);
void write_sequnlock_irq(seqlock_t *lock);
void write_sequnlock_bh(seqlock_t *lock);
```
## 8.2 实现


# 9 RCU (Read-Copy-Update) (arch相关) (高性能锁)
http://www2.rdrop.com/users/paulmck/rclock/
## 

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