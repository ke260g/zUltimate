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
void spin_unlock_bh(spinlock_t *lock); // 释放后还原 bh状态

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
### 5.3.4 spin_lock_bh spin_unlock_bh 的场景
1. 下半部 可以抢占进程上下文; 如果进程
2. 下半部 与 进程有上下文 有共享数据时; 但硬中断回调与进程上下文无共享数据
3. 在进程上下文中；仅关闭 下半部 同时保留硬中断(保留硬件响应性能)