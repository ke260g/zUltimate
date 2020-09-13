[TOC]
# workqueue 数据结构 ( workqueue_struct / pool_workqueue / worker / worker_pool / work_struct )
```c++
// 数据结构关系
// workqueue_struct >
//     {  pool_workqueue > worker_pool 
//        pool_workqueue > 
//        ... }
// worker_pool > { work_struct, work_struct, ... }
// 1. 一个 workqueue_struct 有 NR_CPUS 个 pool_workqueue // <- alloc_percpu(struct pool_workqueue);
// 2. 一个 pool_workqueue 指向一个 worker_pool
// 3. 一个 系统有 NR_CPUS*NR_STD_WORKER_POOLS 个 worker_pool；分别对应一个 worker 线程
// 4. 一个 worker_pool 有多个 work_struct
//
// Note: 1. 实际上 NR_STD_WORKER_POOLS 为2; 0表示低优先级; 1表示高优先级
//       2. 不同的 workqueue 把每个 cpu 的 pool_workqueue 指向相应的 work_pool；从而被遍历
struct workqueue_struct {
    struct pool_workqueue __percpu *cpu_pwqs; /* I: per-cpu pwqs */ // alloc_percpu(struct pool_workqueue);
};
struct pool_workqueue {
    struct worker_pool *pool;  /* I: the associated pool */
    struct workqueue_struct *wq; /* I: the owning workqueue */
};
struct worker {
    struct task_struct *task; /* I: worker task */  // 内核线程; ps | grep kworker
    struct worker_pool *pool; /* I: the associated pool */
};
struct worker_pool {
    struct list_head worklist; /* L: list of pending works */ // work_struct->entry
};
struct work_struct {
    atomic_long_t data;     // 单个回调的上下文 (为啥要用 原子变量)
    struct list_head entry; // 链表; 连着多个 work_struct
    work_func_t func;       // 回调
};
#define for_each_pwq(pwq, wq)              // 一个 workqueue_struct 有很多个 pool_workqueue
#define for_each_pool_worker(worker, pool) // 一个 worker_pool 里面有很多个 work_struct
static LIST_HEAD(workqueues);		/* PL: list of all workqueues */
static DEFINE_IDR(worker_pool_idr);	/* PR: idr of all worker_pools */
static DEFINE_PER_CPU_SHARED_ALIGNED(struct worker_pool [NR_STD_WORKER_POOLS], cpu_worker_pools); // 全局 worker_pool

/**
 * @brief 1. 初始化每个 worker_pool
 *        2. 为每个 worker_pool 初始化其 worker 线程
 *        2. 注册系统内置 workqueue: system_wq; system_highpri_wq; system_long_wq; system_unbound_wq;
 *           system_freezable_wq; system_power_efficient_wq; system_freezable_power_efficient_wq;
 */
static int __init init_workqueues(void); // 初始化入口
/**
 * @brief 1. 如果 worker_pool 没有 work_struct 则"休眠" { 进入TASK_INTERRUPTIBLE; 然后 schedule(); }
 *        2. 如果 worker_pool 繁忙 if (!may_start_working(pool)) 则继续"休眠"
 *        3. 遍历 worker_pool->worklist 的每个 work_struct;  process_one_work
 */
static int worker_thread(void *__worker); // 实体线程; worker->task = kthread_create_on_node(worker_thread，... ); 注册
```

# worker_thread (workqueue 内核线程) (process_one_work 执行入口)
```c++

```

# workqueue 使用
```c++
bool schedule_work(struct work_struct *work);
bool schedule_delayed_work(struct delayed_work *dwork, unsigned long delay); // delay 是 jiffies 单位
bool queue_work_on(int cpu, struct workqueue_struct *wq, struct work_struct *work); // 指定 cpu
void flush_scheduled_work(void); // 函数一直等待; 直到所有 work_struct 完成后才退出; 只能在进程上下文中调用
bool cancel_delayed_work(struct delayed_work *dwork);
bool cancel_delayed_work_sync(struct delayed_work *dwork);
```