[TOC]
调度器本质工作: 决定 哪个进程运行；什么时候运行; 运行多久
+ 调度器历史:
    1. O(1) 调度器; linux-2.5 
    2. CFS  调度器; linux-2.6.23 （完全公平调度算法)

# 1. 多任务系统 (非抢占 vs 抢占)
1. 多任务操作传统: 指能 **同时** **并发地** **交互执行** 多个进程的操作系统
2. 术语:
    1. **让步**  (yielding):   进程主动挂起
    2. **抢占**  (preemption): 进程被动挂起 (通常由 调度器挂起)
    3. **时间片** (timeslice): 进程被抢占前能运行的时间
## 1.1 非抢占式多任务 (Cooperative Multitasking)
1. 除非进程主动挂起 否则进程将一直运行
2. 理论上  进程通常作出让步
3. 这种机制的缺陷:
    1. 调度器无法统计规定 每个进程的执行时间
    2. 进程占有CPU 的时间 超出用户的预料
    3. 某个进程永不让步  导致系统崩溃
+ 属于
## 1.2   抢占式多任务 ( Preemptive Multitasking)
1. 实现一个调度器
2. 由调度器决定何时 挂起一个进程的运行；(以运行其他进程)
    + 动态分配进程的运行时间

# 2. 调度策略 (响应快 vs 吞吐量高)
1. 调度策略 决定 **什么时候** 运行 **什么进程**
2. 调度策略 在两个矛盾的目标中寻找平衡: 响应快 和 吞吐量高
    + 本质矛盾: 切换上下文消耗大量的时间
## 2.1 进程分类 (I/O 消耗型  or CPU 消耗型)
1. 大部分时间用用于 提交 or 等待 I/O请求 (键盘 / 磁盘 / 网络); 即阻塞
    + 例子: 协议服务进程；绝大多数UI程序
2. 大部分时间用于执行代码; 除非被抢占(?时间片?) 否则一直运行
    + 例子: matlab sshkeygen
    + 调度策略(一般是): 降低其调度频率; 延长其运行时间

## 2.2 优先级 (prio  和 nice)
1. 机制
    1. 优先级高的先运行; 优先级低的后运行 (什么时候)
    2. 优先级高的运行长; 优先级低的运行短 (运行多久)
    3. 优先级相同; 则按轮转方式进行调度
2. linux 优先级表征
    1. nice值: 值域`[-20, 19]` 默认 `0`; nice值越低 优先级越高; (ps::nice 列)
    2. 实时优先级: 值域 `[0, 99]`  默认 `50`; (ps::rtpiro列); `MAX_RT_PRIO`
    3. 普通优先级: 值域 `[0, 140]` 默认 `70`; `140 = MAX_RT_PRIO + (MAX_NICE - MIN_NICE + 1)`
3. 内核代码
    1. `task_struct->static_prio` 和 `task_struct->prio`
    2. 设置方法: `set_user_nice`

## 2.3 时间片 timeslice
1. 默认时间片的选取问题:
    1. 太长导致 进程响应变慢; 交互式进程表现不好(GUI卡顿)
    2. 太短导致 频繁切换上下文; 进程时间运行时间变短(吞吐量低)
    3. I/O 消耗型进程 显然不需要过长的时间片
    4. CPU 消耗型进程 时间片越长越好
2. linux 内核实现:
    1. 实际上 进程时间片 由 系统负责 和 优先级决定
    2. 也就是 nice -> 优先级 -> 划分权重 -> 时间片
    3. CFS 调度器不直接分配时间片; 而是划分 CPU 使用比例
3. 技术上 时间片必须是内核节拍器(tick)的整倍数;

# 3. 调度算法
## 3.1 unix 原始调度算法
### 3.1.1 策略 
1. unix 原始进程调度采用 nice值 到 时间片的线性绝对映射
2. 假设该映射关系为: `(100 - nice * 5) ms`
    + 即: (nice:-20 = 200ms) ~ (nice:0 = 100ms) ~ (nice:19 = 5ms)
### 3.1.2 问题
1. 绝对映射 nice值 到 时间片; 单位时间片 太短或太长 都不好
    1. 如果 两进程 nice=19 但少有其他进程; 则系统频繁切换上下文导致吞吐量少
2. 连续nice值 映射的 时间片差别 诡异
    1. 如果 nice=0  和  nice=1; 则时间片分别为 100ms 和 95ms; 时间片相差很少
    2. 如果 nice=18 和 nice=19; 则时间片分别为  10ms 和  5ms; 时间片相差很大
3. 与 "时间片必须节拍器(tick)的整倍数" 不兼容
    1. 最小时间片的尺度受 tick 限制
    2. 时间片随着 tick 改变而改变(影响时间算法)
4. 优先级策略 不好 优化 I/O消耗型的 交互式进程
    1. 直接提高优先级只能提高 时间片; 但实际上进程不需要太长的时间片
## 3.2 linux CFS 公平调度算法
### 3.2.1 策略
1. 时间片 = 进程权重(nice值映射)在全部可运行进程所占比例 / 目标延迟时间
2. 目标延迟时间 = 时间片分母；即进程切换耗时为零时 两次调度同一进程的时间差
    + 实际上; 这个值越大 吞吐量越大; 交互响应速度越慢
3. 最小时间粒度 = 当进程个数作够多时; 时间片不能为零; 因此给定最小时间粒度
### 3.2.2 问题
1. 当进程个数足够多; 每个进程的时间片都接近 最小时间粒度; 该算法无法生效
    + 即退化为 线性绝对映射
2. 当进程个数足够多; 目标延迟时间 也失效
    + n*最小时间粒度  远大于 目标延迟时间

## 3.3 linux rt 实时调度算法
1. 实时调度算法支持两种策略 `SCHED_FIFO` and `SCHED_RR`
### 3.3.1 策略 
1. `SCHED_FIFO` 下 `TASK_RUNNING`的进程 没有时间片 一直在运行; 
    1. 主动阻塞(block-syscall) or 显示调度(调度syscall) 才会抢占
    2. 只有更高优先级的 { `SCHED_FIFO` or `SCHED_RR` } 下 `TASK_RUNNING`的进程才能抢占
2. `SCHED_RR` 形同带时间片的 `SCHED_FIFO`; 进程耗尽预分配的时间片则不再运行
    1. 一个进程停止运行后; 调度器轮流调度其他进程; 重新分配时间片后进程重新运行
    2. 其他特性与 `SCHED_FIFO` 类似
    3. 实时调度算法的两种策略共用 优先级
    4. 即 `SCHED_RR`进程 耗尽时间片; 低优先级的`SCHED_FIFO`进程也不能抢占
### 3.3.2 问题
1. 该实时调度技术上是 "软实时" 而非 "硬实时"
    + 受制于上下文切换?

# 4. 调度器实现
## 4.1 调度器类别 struct sched_class
1. 允许多种可动态添加的调度器算法并存; 调度属于自己范畴的进程
2. 每个调度器有优先级; 基础调度器按照优先级遍历调度器类; 优先级最高的胜出
3. linux 调度器
    1. `kernel/sched/fair.c`       策略: `SCHED_NORMAL`
    2. `kernel/sched/deadline.c`   策略: `SCHED_DEADLINE`
    3. `kernel/sched/idle_task.c`  策略: `SCHED_IDLE`
    4. `kernel/sched/rt.c`         策略: `SCHED_FIFO` and `SCHED_RR`
    5. `kernel/sched/stop_task.c`  策略: 
### 4.1.1 调度器实体（struct sched_entity)
1. 存储位置 `task_struct->se`

## 4.2 时间记账  (以 CFS 为例)
### 4.2.1 机制
1. 每个进程 分配一个时间片
2. 在tick中断回调中 进程时间片减少tick周期
3. 当进程时间片 减少至0; 则被另一时间片不为0的进程抢占
### 4.2.2 记账对象 (运行时间统计)
1. 虚拟运行时间 `sched_entity->vruntime`
    1. 记录进程 已经运行时间 和 还需运行时间
    2. 该变量记录时; 经过"所有可运行进程总数"的标准化
2. 由于经过标准化; vruntime 与 节拍器不再相关
3. "理想多任务处理器" 相同优先级的进程 虚拟运行时间也相同
4. 但是; 由于进程实际上是逐个运行的; 所以需要vruntime记录

### 4.2.3 记账方法 fair.c::update_curr()  (以 CFS 为例)
1. 获取当前时间 `u64 now = rq_clock_task(rq_of(cfs_rq));`
2. 计算执行时间 `u64 delta_exec = now - curr->exec_start;`
3. 更新统计变量 `calc_delta_fair` 可运行进程总数的时间片加权算术
```c++
curr->exec_start = now;

schedstat_set(curr->statistics.exec_max,
            max(delta_exec, curr->statistics.exec_max));

curr->sum_exec_runtime += delta_exec;
schedstat_add(cfs_rq, exec_clock, delta_exec);

curr->vruntime += calc_delta_fair(delta_exec, curr);
update_min_vruntime(cfs_rq);
```
## 4.3 进程选择  (以 CFS 为例)
1. 由于并非运行不能 实现 "理想多任务处理器"
2. CFS 企图均衡同优先级进程的 虚拟运行时间
3. 即 当CFS选择下一个进程时 选择vruntime最小的进程 (??)
4. 实现方法 vruntime作为key 构造 rbtree 存储进程队列
    + 从而快速索引 vruntime 最小的进
### 4.3.1 选择下一个进程 pick_next_entity()
1. `__pick_first_entity()` 选取 vruntime最小的进程 即 rbtree 最左叶
    + 该函数不遍历; 因为 rbtree 实现是缓存了最左叶
2. `__pick_next_entity()`  选取 当前节点的下一个节点 (迭代法遍历)
    1. 获取当前节点的 **右节点的最左叶** or **右节点本身** 
    2. 如果没有右节点则往上遍历; 直到找到首个 父节点 其为祖节点的左节点
    ```c++
        while (parent = rb_parent(node) && node == parent->rb_left)
            node = parent;
    ```
### 4.3.2 插入一个进程 enqueue_entity()
1. 当进程进入 `TASK_RUNNING` or `fork()首次创建` 调用
2. `update_curr()` 更新统计信息(标准化的 vruntime)
3. `update_stats_enqueue()` 更新队列本身的统计信息
4. `__enqueue_entity()` rbtree 方法
    + `rb_insert_color()` 为插入方法; 处理着色逻辑
    + 其中更新保存 rbtree 最左叶
### 4.3.3 删除一个进程 dequeue_entity()
1. 当进程进入 "休眠状态" or 进程销毁 时调用
    + `TASK_INTERRUPTIBLE` or `TASK_UNINTERRUPTIBLE`
2. `update_curr()` 更新统计信息(标准化的 vruntime)
3. `update_stats_dequeue()` 更新队列本身的统计信息
4. `__dequeue_entity()` rbtree 方
    + `rb_erase()` 为删除方法
    + 其中更新保存 rbtree 最左叶

## 4.4 调度器入口 schedule()
1. 内核其他部门 调用进程调度器的入口
2. 该函数 选择哪个进程可以运行; 决定何时运行该进程
    + 由pick_next_task 实现
### 4.4.1 pick_next_task() 选择下一个运行的进程
1. 调用链 `schedule > __schedule > pick_next_task`
2. 策略
    1. 遍历找到最高优先级的调度类 schedule class
    2. 调用调度类的方法决定  下一个运行的进程
    3. 优化: 如果所有进程都在CFS中; 则直接调用 CFS 的方法
3. 调度类实现了 `sched_class->pick_next_task` 方法
    1. CFS 的 pick_next_task 实现就是 `pick_next_task_fair()`
    2. `pick_next_entity()` 由 `pick_next_task_fair()` 调用

## 4.5 休眠和唤醒
1. 休眠通过 "等待队列" 实现
2. 实际上休眠的触发发生 阻塞型系统调用 和 信号接收中
3. 实现休眠需要遵循相当负责的 pathern; 参考具体系统调用
    + 例子: `inotify_read()` 
### 4.5.1 休眠 prepare_to_wait()
1. `DEFINE_WAIT(wait)` 创建"等待队列"节点 `wait_queue_t`
    + + 系统调用需要在 内核初始化时 定义自己的队列头 `wait_queue_head_t`
2. 在 `while(1)` 循环中等待条件符合
3. 循环业务1: 开始等待 `prepare_to_wait()` 
    1. `task_struct->state` 设置为 `TASK_INTERRUPTIBLE` or `TASK_UNINTERRUPTIBLE`
    2. 节点加入 "等待队列" `__add_wait_queue()`；然后阻塞并等待 异步的`wake_up()`
4. 循环业务2: 处理系统调用的业务; 符合条件就终止循环
5. 循环业务3: 提前终止a) `O_NONBLOCK` 时 `errno = -EGAIN` 
6. 循环业务4: 提前终止b) `TASK_INTERRUPTIBLE` 的进程接收到信号 `errno = -EINTR`
7. 循环业务5: 如果条件不满足(`while(1)` 的结尾); 则 `schedule()` 进行调度
4. 退出循环后结束等待 `finish_wait()`
    1. `task_struct->state` 设置为 `TASK_RUNNING`
    2. 节点退出 "等待队列" `list_del_init(&wait->task_list);`
5. 系统调用返回; 出发调度器逻辑; 最后返回到用户态
### 4.5.2 唤醒 wake_up()
1. wake_up 是"等待队列" 的方法; 由 系统调用的业务 唤醒队列上的所有进>程
2. 调用链 `wake_up > __wake_up > __wake_up_common >  wait_queue_t->func`
3. 而 `wait_queue_t->func` 就是 `autoremove_wake_function()`
    + 在 `DEFINE_WAIT(wait)` 定义时赋值
4. 调用链 `autoremove_wake_function > default_wake_function > try_to_wake_up`
    + try_to_wake_up 为唤醒业务逻辑
5. 调用链 `try_to_wake_up`>
    `ttwu_queue`>` ttwu_do_activate`>`ttwu_activate`>
    `activate_task`>`enqueue_task`>`sched_class->enqueue_task`
6. CFS `enqueue_task()` 的实现即 `enqueue_task_fair`；其中调用 `enqueue_entity` 入 rbtree

# 5. 抢占 与 上下文切换
## 5.1 抢占 (用户抢占 / 内核抢占)
1. 用户抢占
    1. 用户抢占 = 返回用户态时 need_resched 被拉起; `schedule()` 切换到新进程
    2. 用户抢占场景 = a) 从软中断-系统调用返回 b) 从硬中断-处理函数返回 
2. 内核抢占
    1. 只要 **重新调度是安全的** 内核态即可被抢占
    2. **重新调度是安全的** = 没有持有锁; 内核即可进行抢占
3. 经验上来看; 内核开发中很 bug 是因为锁用错了导致; 关闭内核抢占可以临时规避

## 5.2 切换逻辑入口 context_switch = switch_mm + switch_to
1.  context_switch() 上下文切换 主入口
1. 调用栈
```c++
context_switch
    < __schedule
        < schedule
        < preempt_schedule
        < preempt_schedule_irq
        < __cond_resched
            < _cond_resched
```
2. context_switch 的主要切换工作在于 `switch_mm_irqs_off` 和 `switch_io`
    1. 目前只有 x86 实现了 `switch_mm_irqs_off` 
    2. 其他arch 都是 `#define switch_mm_irqs_off switch_mm`
    3. 实际上 `switch_mm()` 在调度器其他场景被调用
    4. 而 x86 的 `switch_mm()` 中调用的是 `switch_mm_irqs_off()`
3. switch_mm() (切换进程的虚拟内存)
4. switch_to() (切换进程的处理器状态)
    + 保存(当前进程) 和 恢复(就绪进程) 的 栈地址 和 寄存器信息

## 5.3 need_resched 调度判定标记
1. 机制
    1. 每个进程都有 need_resched标记 `task_struct->need_resched`
    2. 在特定场景被拉起; 内核在??检查为true 则 `schedule()` 切换到新进程

2. 场景 (啥时候需要重新调度)

| 函数 | 场景 |
| :--  | :--  |
| `update_process_times()` | 定时器中断回调 |
| `try_to_wake_up()` | 优先级高的进程进入可执行状态时 |
| `没找到()` | 返回用户态时 |
| `没找到()` | 离开中断回调 |
3. 方法
    1. `set_tsk_need_resched()`   set 拉起标记；(一般用 `resched_task()`封装)
    2. `clear_tsk_need_resched()` set 清除标记;
    3. `test_tsk_need_resched()`  get 读取标记;
4. CFS 调用链 (不同场景的调用链)
```c++
// 定时器中断回调
update_process_times > scheduler_tick > current->sched_class->task_tick
    > task_tick_fair // 以 CFS 公平调度算法为例
    > entity_tick
        > update_curr 
            > account_cfs_rq_runtime > __account_cfs_rq_runtime > resched_task
        > check_preempt_tick > resched_task
```
+ 用 CFS 作例子 从 resched_task 的引用反推 调用链; 对照 "场景"

# 6. 调度相关 syscall (sched.h)
| syscall | explanation |
| :-: | :-- |
| nice | 设置nice值 |
| sched_setscheduler | set 调度器策略; 传入参数 policy 为 `SCHED_*` |
| sechd_getscheduler | get 调度器策略 |
| sched_setparam | set 调度器参数；目前只有实时优先级 |
| sched_getparam | get 调度器参数 |
| sched_setaffinity | set 处理器亲和力；即 可运行的处理器 |
| sched_getaffinity | get 处理器亲和力 |
| sched_rr_get_interval | get 进程的时间片 |
| sched_yield | 用户态显式 让出CPU; 即主动抢占 |