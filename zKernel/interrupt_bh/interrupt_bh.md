[TOC]
中断下半部本质上有5种方式处理: sortirqs, tasklet, workqueue, timer(这里不讨论; 放到时间相关讨论), 内核线程(这里不讨论)

# 3. Tasklets
+ 一些特性
    1. 运行在中断上下文中
    2. 
## 3.1 方法 ( 创建/使能/提交 )
```c++
#define DECLARE_TASKLET(name, func, data) 
#define DECLARE_TASKLET_DISABLED(name, func, data)
void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long),  unsigned long data);

void tasklet_disable(struct tasklet_struct *t); // 阻塞直到停止
void tasklet_disable_nosync(struct tasklet_struct *t);
void tasklet_kill(struct tasklet_struct *t);
void tasklet_enable(struct tasklet_struct *t);

void tasklet_schedule(struct tasklet_struct *t);
void tasklet_hi_schedule(struct tasklet_struct *t);
```
## 3.99 hi_schedule 与 schedule 的区别

# 4. workqueue
## 4.1 方法 (workqueue = {创建/停止/销毁} work = { 定义/提交/停止 } )
```c++
// workqueue 方法
#define create_workqueue(name)	            // 每个CPU上都分配一个
#define create_singlethread_workqueue(name)	// 仅运行于一个CPU
void flush_workqueue(struct workqueue_struct *wq);
void drain_workqueue(struct workqueue_struct *wq);
void destroy_workqueue(struct workqueue_struct *wq);

// work 方法
#define DECLARE_WORK(n, f)      // 编译时 静态定义
#define INIT_WORK(_work, _func) // 运行时 初始化
bool queue_work(struct workqueue_struct *wq, struct work_struct *work); // 提交到指定workqueue
bool schedule_work_on(int cpu, struct work_struct *work); // 提交到指定cpu
bool schedule_work(struct work_struct *work); // 提交到内核自带 system_wq

bool flush_work(struct work_struct *work);  // 阻塞直到 work 运行完成; 无论work在pending/flight
bool cancel_work(struct work_struct *work); // 异步取消; 取消pending状态的work; flight状态的直接返回
bool cancel_work_sync(struct work_struct *work); // 同步取消; 取消pending状态的work; flight状态 阻塞直到 work运行完成
```
