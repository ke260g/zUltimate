[TOC]
# 1. schedule
```c++
extern signed long schedule_timeout(signed long timeout);
extern signed long schedule_timeout_interruptible(signed long timeout);
extern signed long schedule_timeout_killable(signed long timeout);
extern signed long schedule_timeout_uninterruptible(signed long timeout);
asmlinkage void schedule(void);
extern void schedule_preempt_disabled(void);
```

# 2. wait
```c++
wait_event_timeout();
```

# 2. timer
SMP 架构中; 同一个timer尽量让同一个CPU运行; 以有效利用 L2Cache
in_atomic() 与 in_interrupt() 与 in_irq() 的区别??
+ 原始性能需求:
    1. Timer management must be as lightweight as possible. (提高实时性)
    2. he design should scale well as the number of active timers increases. (运行在系统中的定时器是海量的)
    3. Most timers expire within a few seconds or minutes at most, while timers with long delays are pretty rare.
    4. A timer should run on the same CPU that registered it. (SMP架构中, 有效利用CPU-Cache)
## 2.1 方法 ( 创建 / 增加 / 销毁 )
```c++
#define DEFINE_TIMER(_name, _function, _expires, _data)
#define init_timer(timer)

// 设置属性: TIMER_DEFERRABLE, TIMER_PINNED can be used Or-bit
void timer_setup(struct timer_list *timer, void (*callback)(struct timer_list *), unsigned int flags);
// 增删改
void add_timer(struct timer_list *timer);
int del_timer(struct timer_list * timer);
// expires > 0 && 没有激活; mod_timer会激活timer
int mod_timer(struct timer_list *timer, unsigned long expires);

/* mod_timer 会自动激活没有激活的定时器回调;
 * mod_timer 更常见的用法是 在 回调执行完后再次 mod_timer(timer, jiffies + ms_to_jiffies(1000));
 * 从而实现循环定时器
 */
```
## 2.2 实现 (为了满足原始性能需求)
+ internal_add_timer
+ update_process_times > run_local_timers > raise_softirq(TIMER_SOFTIRQ); > run_timer_softirq > __run_timers
## 2.3 调试