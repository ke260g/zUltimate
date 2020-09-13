[TOC]
实际上; 除了部分内核 和 操作系统性能要求较高的业务(协议栈 定时器)等
更多时候；使用tasklet更合适

1. 禁止同一 tasklet 并发执行
# tasklet
tasklet 的三个周期: `内核初始化 > 用户调度 > 软中断线程执行`
tasklet 高优先级如何体现? 在softirq数组中排得最靠前
tasklet 如何实现 smp 互斥 (禁止并发)?  原子变量加锁 TASKLET_STATE_RUN
## tasklet 定义
```c++
struct tasklet_struct {
    struct tasklet_struct *next; // tasklet 以链表形式存在
    unsigned long state;         // TASKLET_STATE_*; 锁
    atomic_t count;              // 引用计数
    void (*func)(unsigned long); // 回调
    unsigned long data;          // 回调自身的上下文
};

// tasklet_struct->state; 中有两个锁; 每个bit一个; 分别用原子访问
// 实际上只有 smp 架构中; 另一个cpu才能看到 TASKLET_STATE_RUN;
// 单核cpu 架构中; 只能看到 TASKLET_STATE_SCHED
// TASKLET_STATE_SCHED: 单 cpu 避免重复调度
// TASKLET_STATE_RUN:    cpu 间实现互斥; 
enum {
    TASKLET_STATE_SCHED, /* Tasklet is scheduled for execution */
    TASKLET_STATE_RUN    /* Tasklet is running (SMP only) */
};

void __init softirq_init(void) {
    int cpu;
    for_each_possible_cpu(cpu) {
        per_cpu(tasklet_vec, cpu).tail = &per_cpu(tasklet_vec, cpu).head;
        per_cpu(tasklet_hi_vec, cpu).tail = &per_cpu(tasklet_hi_vec, cpu).head;
    }
    open_softirq(TASKLET_SOFTIRQ, tasklet_action);
    open_softirq(HI_SOFTIRQ, tasklet_hi_action);
}
```
1. tasklet 的初始化在 软中断初始化中
2. 显然; 每个cpu 有自己的tasklet 队列

## tasklet 调度 （ tasklet_schedule / tasklet_hi_schedule )
```c++
#define DECLARE_TASKLET(name, func, data)
#define DECLARE_TASKLET_DISABLED(name, func, data)

static inline void tasklet_schedule(struct tasklet_struct *t) {
    // 如果已经调度了; 则马上返回;
    // test_and_seet_bit 是原子操作; 从而实现 smp 间的互斥
    if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) 
        __tasklet_schedule(t);
}

void __tasklet_schedule(struct tasklet_struct *t) {
    unsigned long flags;

    local_irq_save(flags);  // 关闭本地中断; 
    t->next = NULL;
    *__this_cpu_read(tasklet_vec.tail) = t;
    __this_cpu_write(tasklet_vec.tail, &(t->next));
    raise_softirq_irqoff(TASKLET_SOFTIRQ);
    local_irq_restore(flags);
}
EXPORT_SYMBOL(__tasklet_schedule);
```
1. `tasklet_schedule` 原子访问 `tasklet_struct->state` 实现互斥
2. 关闭本地中断 防止入列操作 和 唤醒操作 因为中断导致数据错乱

## tasklet 执行 ( tasklet_action / tasklet_hi_action )
```c++
static void tasklet_action(struct softirq_action *a) {
    struct tasklet_struct *list;

    // 1. 与入列同样道理; 出列操作避免 数据错乱要关闭中断
    // 2.1 这里是一个trick；把一整个队列(单向链表)取出来
    // 2.2 实际上 初始化时也是 { head = NULL; tail = &head; }
    local_irq_disable();
    list = __this_cpu_read(tasklet_vec.head);
    __this_cpu_write(tasklet_vec.head, NULL);
    __this_cpu_write(tasklet_vec.tail, &__get_cpu_var(tasklet_vec).head);
    local_irq_enable();

    while (list) {
        struct tasklet_struct *t = list;
        list = list->next;

        if (tasklet_trylock(t)) {   // 实现 smp 互斥 加锁
            if (!atomic_read(&t->count)) {
                if (!test_and_clear_bit(TASKLET_STATE_SCHED, &t->state)) // 重新允许本 cpu 调度
                    BUG();
                t->func(t->data);
                tasklet_unlock(t);  // 重新允许其他 cpu 执行
                continue;
            }
            tasklet_unlock(t);
        }
        // 1. 与入列同样道理; 出列操作避免 数据错乱要关闭中断
        // 2. 加锁失败; 说明正在被其他 cpu 运行
        // 3. 重新入列; 插入回本 cpu 的队列
        local_irq_disable(); 
        t->next = NULL;
        *__this_cpu_read(tasklet_vec.tail) = t;
        __this_cpu_write(tasklet_vec.tail, &(t->next));
        __raise_softirq_irqoff(TASKLET_SOFTIRQ);
        local_irq_enable();
    }
}

// tasklet->state 的 TASKLET_STATE_RUN 锁
#ifdef CONFIG_SMP
static inline int tasklet_trylock(struct tasklet_struct *t) {
	return !test_and_set_bit(TASKLET_STATE_RUN, &(t)->state);
}
static inline void tasklet_unlock(struct tasklet_struct *t) {
    smp_mb__before_atomic();
    clear_bit(TASKLET_STATE_RUN, &(t)->state);
}
#else
#define tasklet_trylock(t) (1)
#define tasklet_unlock(t) do { } while (0)
#endif
```
1. 体现了 中断禁止 与 加锁 的典型使用
    1. 本 cpu 数据一致性通过 中断禁止 保护
    2. cpu 间数据一致性通过加锁保护