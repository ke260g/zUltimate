[TOC]
中断分层, 处理硬件中断分两个部分处理, 上部分 和 下部分:
https://blog.csdn.net/MyArrow/article/details/9287169
上半部(硬件相关): top half
  中断发生后, 优先处理硬件读写, 注册该中断(放在全局容器?), 在irq_handler处理
  完成尽可能少的比较紧急的功能，往往只读取寄存器中的中断状态并清除中断标志
  然后就进行“登记中断”(也就是将底半部处理程序挂在到设备的底半部执行队列中的工作）
  特点：响应速度快
下半部(硬件无关): bottom half
  在CPU idle时作后续处理(处理全局容器中的对象?)
  中断处理的大部分工作都在底半部，它几乎做中断处理程序的所有事情
  处理相对来说不是非常紧急的事件
下半部的实现:
  bottom half 接口:
      编译前静态创建, 链表, 共32个节点
      上半部通过一个32位整数中的一位来标识出哪个bottom half可执行
      每个BH都在全局范围内进行同步，即使分属于不同的处理器
      也不允许任何两个bottom half同时执行
  work queues:
      一组队列, 每个队列都包含一个由等待调用的函数组成链表
      根据其所处队列的位置，这些函数会在某个时刻被执行，
      驱动程序可根据需要把它们自己的下半部注册到合适的队列上去
  tasklets:
      动态创建
      两个不同类型的tasklet可以在不同的处理器上同时执行
      两个类型相同的tasklet不能同时执行
  softirqs(不是系统调用  TRAP):
      编译前静态创建, 32个，可以在所有的处理器上同时执行 (即使两个类型完全相同)
举例:
  比如用户敲击键盘, 然后把键盘输入到user态程序,
  响应敲击键盘, 往往击键一次,
  上半部, 先进行记录
  下半部, 在空闲的时候把击键传输到用户层

开发经验::必须在上半部处理的场景
1.如果一个任务对时间非常敏感，感觉告诉我还是将其放在中断处理程序中执行是个好的选择。
2.如果一个任务和硬件相关，还是将其放在中断处理程序中执行吧。
3.如果一个任务要保证不被其他中断(特别是相同的中断)打断，那就将其放在中断处理程序中吧。
4.其他所有任务，除非你有更好的理由，否则全部丢到下半部执行。
test_and_change_bit(); // 读取 和 取反
test_and_set_bit();    // 读取 和 置1
test_and_clear_bit();  // 读取 和 清0

## 原子操作
原子操作的本质:
1. 避免多CPU同时操作
2. 避免在操作时被硬件中断
```c++
unsigned long flags;
local_irq_save(flags);    // 关闭当前CPU中断, 保存之前的中断状态
/* 这之间进行 原子操作 */
local_irq_restore(flags); // 开启当前CPU中断, 保存之前的中断状态

local_irq_disable();  // 关闭当前CPU中断
/* 这之间进行 原子操作 */
local_irq_enable();   // 开启当前CPU中断
```
Note: enable/disable 是强制的, 不会保存之前中断状态 </br>
      需要调用者确保 该中断上下文中没有其他中断 </br>
      否则一律用 save/restore
+ 使用 save/restore:   do_softirq, raise_softirq, tasklet_schedule, tasklet_hi_schedule
+ 使用 enable/disable: tasklet_action, tasklet_hi_action
+ 实际上必须先使用 save/restore, 而在 {save,restore} 之间使用 enable/disable

## interrupt
任何一个中断系统, 至少要有的抽象: 描述符, 回调原型, 注册接口, 触发接口
三套中断系统: BH > softirqs > tasklet > work queue

了解重点:
+ 每个系统的生命周期: (中断没有销毁 这一生命周期)
  + fatal: 注册, 执行
  + optional: 使能, 禁止
  + 更多的是具体系统相关, 如tasklet "调度"
+ 功能的异同, 即实现了何种需求, 反映了实际硬件设备的物理特性 以及 性能考虑
+ 了解每一个系统 如何实现 需求(如 Who marks, Who runs)
+ 是否中断嵌套: 上层回调是否需要实现"串行化"
+ 是否允许休眠: 休眠指阻塞函数(申请内存,信号量,某些锁)

### BH
1. 在任意一时刻，系统只能有一个CPU可以执行BH代码，</br>
  以防止两个或多个CPU同时来执行BH函数而相互干扰。 </br>
  因此BH代码的执行是严格“串行化”的
2. BH函数不允许嵌套
+ 两个特点, 导致BH无法充分发挥SMP硬件的优势, 一个时刻只有一个中断在进行

### softirq
softirq机制是与SMP紧密不可分的, Who marks，Who runs.
each CPU 有它自己的软中断触发与控制机制, 只运行当前CPU触发的中断
同一个历程routine(代码段), 可以被多个CPU并发执行
软中断号只有32个, 且内核初始占用了10个 (linux-3.16.78)
```C++
enum { XX__SOFTIRQS, NR__SOFTIRQS }; // 中断ID
struct softirq_action
{
	void	(*action)(struct softirq_action *);
};
struct softirq_action softirq_vec[NR_SOFTIRQS]; // 全局软中断矢量, 全CPU共享

// 触发, nr为中断ID, XX__SOFTIRQS
// raise 触发中断, 然后执行 softirq_ver[nr].action
void raise_softirq(unsigned int nr)；
inline void raise_softirq_irqoff(unsigned int nr);

// 注册, nr为中断ID, action为中断处理函数
void open_softirq(int nr, void (*action)(struct softirq_action *)); 
// 执行                                    
void do_softirq(void);               

// 每个CPU有ta自己的一个上下文, irq_cpustat_t
#define NR_IPI	6 // CPU个数
  
typedef struct {
  unsigned int __softirq_pending;
#ifdef CONFIG_LOCAL_TIMERS
  unsigned int local_timer_irqs;
#endif
#ifdef CONFIG_SMP
  unsigned int ipi_irqs[NR_IPI];
#endif
} ____cacheline_aligned irq_cpustat_t;

irq_cpustat_t irq_stat[NR_CPUS];
```
每个CPU虽然都有它自己的触发和控制机制，
并且只执行他自己所触发的软中断请求，
但是各个CPU所执行的软中断服务例程却是相同的
结论: 某一段softiqrs 同一时刻可以在不同CPU并发运行

### tasklet
+ tasklet 历史上是为了把 旧时代BH接口 移植到softirq机制中
  + 与BH相同, 中断不嵌套
  + 与softirqs不同, 某一段tasklet 同一时刻只能在一个CPU上运行
  + 与BH接口不同,   不同段tasklet 不同时刻在不同CPU上运行
+ tasklet 本质上是一个特殊的 softirq
  + softirq 中的 HI_SOFTIRQ 和 TASKLET_SOFTIRQ 均是用tasklet机制来实现的
  + 触发中断的接口: 先处理tasklet逻辑, 再调用softirqs原始的raise接口
+ 一个tasklet至少3个标记位:
  1. 使能/禁止
  2. 是否 正在被执行 (实现 某一段tasklet 同一时刻只能在一个CPU上运行)
  3. 是否 已调度-未执行-但正等待
+ tasklet 可以 加入队列中, 执行后即出列
+ tasklet 已经在 队列中, 可以 使能/禁止
+ tasklet 已经在 队列中, 可以 停止调度

```c++
struct tasklet_head // 单向不成环 链表
{
	struct tasklet_struct *head;
	struct tasklet_struct **tail; // 指向尾部对象的next
};
struct tasklet_struct
{
	struct tasklet_struct *next;  // 链表句柄
	unsigned long state;          // bit[0]: 已调度未执行 bit[TASKLET_STATE_SCHED]
                                // bit[1]: 正在被执行   bit[TASKLET_STATE_RUN]
	atomic_t count;               // 引用计数值, 用于使能/禁止
	void (*func)(unsigned long);  // 回调
	unsigned long data;           // 回调参数, 可用于表示userspace的指针 
};

// state-bit[TASKLET_STATE_RUN] = 1, 加锁, 加锁前先检查
static inline int tasklet_trylock(struct tasklet_struct *t);
// state-bit[TASKLET_STATE_RUN] = 0, 解锁
static inline void tasklet_unlock(struct tasklet_struct *t); 
// 一直等待 state-bit[TASKLET_STATE_RUN] 为 0, 用 barrier() 等待
static inline void tasklet_unlock_wait(struct tasklet_struct *t);

// 简单禁止, count--
static inline void tasklet_disable_nosync(struct tasklet_struct *t);
// 简单禁止, 然后 tasklet_unlock_wait
static inline void tasklet_disable(struct tasklet_struct *t);

// 简单使能, count++
static inline void tasklet_disable_nosync(struct tasklet_struct *t);
// 简单使能, 然后 tasklet_unlock_wait
static inline void tasklet_disable(struct tasklet_struct *t);

void tasklet_init(struct tasklet_struct *t,
		  void (*func)(unsigned long), unsigned long data);

// tasklet 是 softirqs的 HI_SOFTIRQ 和 TASKLET_SOFTIRQ 的一种实现
// 所以, 也贯彻 Who marks, Who runs, 即保证每个CPU有其自己的矢量
static DEFINE_PER_CPU(struct tasklet_head, tasklet_vec);
static DEFINE_PER_CPU(struct tasklet_head, tasklet_hi_vec);
// 宏展开后
struct tasklet_head tasklet_vec[NR_CPUS] __cacheline_aligned;
struct tasklet_head tasklet_hi_vec[NR_CPUS] __cacheline_aligned;

// 为啥说 tasklet 是 softirqs 的 HI_SOFTIRQ TASKLET_SOFTIRQ 一种实现?
void open_softirq(int nr, void (*action)(struct softirq_action *))
{
	softirq_vec[nr].action = action;
}
void __init softirq_init(void) {
  // 其他初始化操作 ...
    open_softirq(TASKLET_SOFTIRQ, tasklet_action);
	open_softirq(HI_SOFTIRQ, tasklet_hi_action);
}
// 即 tasklet_action, tasklet_hi_action为 两个软中断ID的回调

/* Q: HI_SOFTIRQ 和 TASKLET_SOFTIRQ 相对其他 softirqs 有哪些不同?
 * A: 有指定的中断触发函数, 先处理tasklet的逻辑
 *    然后分别 raise_softirq_irqoff(TASKLET_SOFTIRQ);
 *            raise_softirq_irqoff(HI_SOFTIRQ);
 *    从而实现一种特殊的 softirqs
 * details:
 *   if (!test_and_set_bit(TASKLET_STATE_SCHED)) // 还没有被调度
 *   local_irq_save();                   // 实现中断不嵌套 -- begin
 *   插入 tasklet 到 tasklet_vec 的尾部, 入列
 *   raise_softirq_irqoff(TASKLET_SOFTIRQ or HI_SOFTIRQ);
 *     然后调用 tasklet_action or tasklet_hi_action 回调
 *   local_irq_restore();                // 实现中断不嵌套 -- end
 */
static inline void tasklet_schedule(struct tasklet_struct *t);
static inline void tasklet_hi_schedule(struct tasklet_struct *t);

/* 回调本身 tasklet_action tasklet_hi_action 的执行思路
 * 1. 原子性 取出 tasklet_vec头, 一个链表指针, 把 tasklet_vec头置NULL 
 * 2. 遍历 tasklet->list 链表
 * 3.   if tasklet_trylock                            // 保证只能一个CPU执行
 * 4.      if tasklet->count == 0                     // 引用计数器, > 1表示使能
 * 5.         test_and_clear_bit(TASKLET_STATE_SCHED) // 关闭 调度等待, 对应schedule中开启调度等待
 * 6.         tasklet->func                           // 调度
 * 7.         tasklet_unlock                          // 执行完 中断代码, 解锁
 * 8.         continue                                // 下一层遍历
 * 9.      else tasklet_unlock                        // tasklet禁止运行, 解锁
 * 10.  把 禁止的/被锁的 tasklet 重新入列 (也就是说 没重新入列的, 就已经出列了)
 */
 static void tasklet_action(struct softirq_action *a);
 static void tasklet_hi_action(struct softirq_action *a);

// kill 的目标: state-bit[0] state-bit[1] 都置0
// kill 的实现: 等待 state-bit[0]为0, 然后置1, tasklet_unlock_wait, 最后state-bit[0]置0
// 即先等待调度完, 再等待执行完, 最后关停调度
// 对应: tasklet_schedule:调度 > 
//      tasklet_action:  等待加锁 > 关闭调度 > 执行" 的过程
void tasklet_kill(struct tasklet_struct *t);

// 2019-12-06: 不是很懂 taslet_kill, 
// 我感觉没啥用呀, 没能阻止 已调度-未执行的 task呀
```

### work queue
+ 由一个内核线程去执行
+ 允许被重新调度和休眠
+ 如何选择 tasklet 和 workqueue
  + task需要休眠，  选择workqueue(如申请内存,信号量,阻塞IO)
  + task不需要休眠, 选择tasklet
+ 实际上, 如果把不需要休眠的task "强行"放到workqueue中, 会影响性能</br>
  因为work queue由内核线程执行, 该线程优先级低于 用户进程调度
+ https://blog.csdn.net/weixin_38227420/article/details/79335702
```c++
typedef void (*work_func_t)(struct work_struct *work); // 回调原型

struct work_struct {
	atomic_long_t data;     // 传入回调的上下文
	struct list_head entry; // 列表结构
	work_func_t func;       // 回调函数
#ifdef CONFIG_LOCKDEP
	struct lockdep_map lockdep_map;
#endif
};

#define DECLARE_WORK(n, f)	    // 声明且初始化
#define INIT_WORK(_work, _func) // 动态初始化

// 入列 system_wq
int schedule_work(struct work_struct *work);
// 入列 wq
int queue_work(struct workqueue_struct *wq, struct work_struct *work);

// 至少等待一定时间后执行
#define DECLARE_DELAYED_WORK(n, f)      // 声明且初始化
#define INIT_DELAYED_WORK(_work, _func) // 动态初始化
struct delayed_work {
	struct work_struct work; // 延时的task
	struct timer_list timer;
};
// 延时入列函数
int schedule_delayed_work(struct delayed_work *dwork, unsigned long delay/*jiffies*/);

// 创建队列
#define create_workqueue(name)
#define create_singlethread_workqueue(name)
// 销毁队列
void destroy_workqueue(struct workqueue_struct *wq);
// 强制执行, 等待所有task执行完 [需要深刻了解]
// 该函数会阻塞, 深入了解一下 color机制
// 如果, task有while, 该函数可能会卡住
// 如果, task在阻塞休眠, 需要先 wake_up_interruptible, 再flush
void flush_workqueue(struct workqueue_struct *wq); 

// 内核全局队列
extern struct workqueue_struct *system_wq;
extern struct workqueue_struct *system_highpri_wq;
extern struct workqueue_struct *system_long_wq;
extern struct workqueue_struct *system_unbound_wq;
extern struct workqueue_struct *system_freezable_wq;
extern struct workqueue_struct *system_power_efficient_wq;
extern struct workqueue_struct *system_freezable_power_efficient_wq;
```