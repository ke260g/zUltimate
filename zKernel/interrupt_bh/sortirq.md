[TOC]
# softirq 定义
```c++
// 类型
struct softirq_action {
    void (*action)(struct softirq_action *); // 回调实体
};

// 对象 (软中断表)
static struct softirq_action softirq_vec[NR_SOFTIRQS] __cacheline_aligned_in_smp;

// 注册 软中断回调; 除了赋值就没其他了
void open_softirq(int nr, void (*action)(struct softirq_action *));

// 触发指定软中断
void __raise_softirq_irqoff(unsigned int nr);  // 触发指定软中断; 拉起 pending 标记位
void raise_softirq_irqoff(unsigned int nr);    // 触发指定软中断; 如果在非中断上下文中 则唤醒内核线程 ksoftirqd (用于中断上下文其他中断被禁止时 local_irq_save)
void raise_softirq(unsigned int nr);           // 触发指定软中断；关中断后再执行action(); 用于非中断上下文

// 软中断入口: a) 硬中断返回 b) ksoftirqd c) 显式调用action方法
// `raise_*()' 接口拉起pending标记
void do_softirq(void); // return __do_softirq(); 遍历软中断表  对pending的调用 `softirq_vec[i]->action()`

// 3.16.78 内核注册的软中断; 10 个
enum {
    HI_SOFTIRQ=0,   // 0: 高优先级tasklet
    TIMER_SOFTIRQ,  // 1: 定时器
    NET_TX_SOFTIRQ, // 2: 发包
    NET_RX_SOFTIRQ, // 3: 收包
    BLOCK_SOFTIRQ,  // 4:
    BLOCK_IOPOLL_SOFTIRQ,
    TASKLET_SOFTIRQ, // 6: 低优先级tasklet
    SCHED_SOFTIRQ,   // 7:
    HRTIMER_SOFTIRQ, // 8:
    RCU_SOFTIRQ,    /* Preferable RCU should always be the last softirq */
    NR_SOFTIRQS
};
```
1. 事实上; `raise_softirq_irqoff` 所谓唤醒内核线程 不是马上实时调度的
2. 而是 `sched_class->task_waking` > `ttwu_queue`; 提高一下响应延迟 (在调度器插队)

## 软中断入口 do_softirq -> __do_softirq
```c++

```

# ksoftiqrd
1. 每个处理器都有一个 ksoftirqd `ps aux | grep ksoftirqd | grep -v grep`
2. 软中断上下文中 允许响应硬中断; 但软中断中不能休眠;
3. 硬中断上下文中; 软中断被禁止; 但其他cpu依然可以处理其他软中断
4. 软中断支持并发, 当一个软中断执行时再次被触发; 那么另一个cpu可以执行同一个软中断

```c++
DECLARE_PER_CPU(struct task_struct *, ksoftirqd); // 进程对象

// smp 内核线程; 即 percpu 线程
// 形同一个进程 while (1) { if (smp_thr->thread_should_run()) smp_thr->thread_fn(); }
static struct smp_hotplug_thread softirq_threads = {
	.store			= &ksoftirqd,
	.thread_should_run	= ksoftirqd_should_run,
	.thread_fn		= run_ksoftirqd,
	.thread_comm		= "ksoftirqd/%u",
};

// 初始化时初始化; ksoftirqd 线程
static __init int spawn_ksoftirqd(void) {
	register_cpu_notifier(&cpu_nfb);
	BUG_ON(smpboot_register_percpu_thread(&softirq_threads)); // 注册
	return 0;
}

static int ksoftirqd_should_run(unsigned int cpu) { // 判断有 pending 的软中断需要执行
	return local_softirq_pending();                 // this_cpu_read(irq_stat.__softirq_pending) 掩码; 形同信号
}

/*
 * 正是因为在 run_softirqd 中会关掉本地
 */
static void run_ksoftirqd(unsigned int cpu)
{
    local_irq_disable(); // 软中断执行过程中; 关掉本地cpu中断
    if (local_softirq_pending()) {
        /*
         * We can safely run softirq on inline stack, as we are not deep
         * in the task stack here.
         */
        __do_softirq();  // 遍历vec表 前会开启本地中断; 遍历完成后关掉
        local_irq_enable();
        cond_resched();

        preempt_disable();
        rcu_note_context_switch(cpu);
        preempt_enable();
        return;
    }
    local_irq_enable(); // 软中断执行结束后; 开启本地cpu中断
}
```

# 软中断设计策略
## 1. 平衡吞吐量 和 用户响应 __do_softirq
1. 单次 `__do_softirq()` 遍历 vec表后; 检查新触发的软中断时间;
2. 在一定间隔内; 重新遍历;
3. 单次 `__do_softirq()` 中可能会遍历多次 vec表; 但是比对的原始时间时固定的
    + 即随着时间推移; 一定能从 `__do_softirq()` 中返回
## 2. percpu 变量实现并发
1. 由于软中断本身是可以多处理器并发;
2. 如果用变量加锁实现并发; 软中断的性能将降低
3. 如果直接禁止单个软中断并发; 性能也将降低
4. 因此, 软中断很多时候使用 percpu 变量实现高性能并发
5. 实际上, tasklet 是基于 "禁止单个软中断并发" 而诞生的