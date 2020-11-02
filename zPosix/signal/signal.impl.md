[TOC]
简单介绍 https://my.oschina.net/u/4280386/blog/4238399

# 对象结构
```c++
struct task_struct {
	struct signal_struct *signal;    // 信号量结构, 大部分参数都在该结构中 (共享的)
	struct sighand_struct *sighand;  // 每个信号处理句柄, 保护信号的自旋锁 (共享的)
	sigset_t blocked, real_blocked;  // 进程阻塞信号 bitmap
	sigset_t saved_sigmask;	         // 进程信号掩码 bitmap
	struct sigpending pending;       // 线程挂起队列, 用于索引挂起的信号
}

// 信号回调
struct sighand_struct {
    atomic_t count;
    struct k_sigaction action[_NSIG]; // 保存信号回调数组 (每个信号对应一个回调)
    spinlock_t siglock;  // 自旋锁; 解决多线程往同一个进程发信号的冲突
    wait_queue_head_t signalfd_wqh;
};

// 信号本身
struct signal_struct { // 这个对象访问的锁; 由 `sighand_struct *sighand' 提供
    struct sigpending shared_pending; // 共享信号挂起队列, 多线程共享
    // ...
};

// 挂起信号队列
struct sigpending {
    struct list_head list; // 指向 struct sigqueue 
    sigset_t signal;       // 当前挂起的信号量位图
};

struct sigqueue {
    struct list_head list; // 两两相连; 头指向 sigpending
    int flags;
    siginfo_t info;        // 该挂起信号的信息
    struct user_struct *user; 
};

//描述信号相关信息的结构体
typedef struct siginfo {
    int si_signo;
    int si_errno;
    int si_code;
    // ...
} siginfo_t;

// 主要核心入口
SYSCALL_DEFINE2(kill, pid_t, pid, int, sig);
SYSCALL_DEFINE2(signal, int, sig, __sighandler_t, handler);
SYSCALL_DEFINE3(sigaction, int, sig, const struct old_sigaction __user *, act, struct old_sigaction __user *, oact);
```
1. `task_struct->(sighand_struct *sighand)->siglock` 还保护着 `task_struct->(signal_struct *signal)`
2. 每个线程有自己的信号队列 `task_struct->pending` 所以这个是变量而不是指针
3. 多线程也有共享的信号队列 `task_struct->signal->pending`, 因为多线程要共享, 所以是指针
4. 多线程必须共享的信号回调 `task_struct->sighand` 因为多线程共享, 所以是指针
    + 同时也说明, 多进程的信号回调, 在同一时间内是相同的

# 信号注册 sigaction 的实现


# 信号发送 kill 的实现
1. kill > kill_something_info > kill_pid_info >
    { pid_task, // 通过pid索引 task_struct; rcu_hlist
      group_send_sig_info } > 
    { check_kill_permission, // 检查发送进程 到 接收进程的权限
      do_send_sig_info } >  send_signal > __send_signal
    { prepare_signal,
      __sigqueue_alloc, // 申请 struct sigqueue; 加入一个信号到 task_struct->sigpending 链表
      signalfd_notify,
      sigaddset(&pending->signal, sig); // 拉起 task_struct->sigpending 中的 bit
      complete_signal
    }
2. `prepare_signal` 判断信号是否需要发送 和 { SIGSTOP  SIGCONT } 预处理
    1. 对于即将退出的进程, 除SIGKILL外都不发送信号
    2. 如果是SIGSTOP, 需先将进程 pending 的SIGCONT信号 移出挂起队列
    3. 如果是SIGCONT, 需先将所有停止信号都移出挂起队列同时清除线程标记位
    4. 判断信号是否需要忽略, 阻塞的信号不忽略, 忽略处理句柄为空与内核认为需要忽略信号
3. `complete_signal`
    1. 寻找可唤醒的线程 
    2. 如果信号被阻塞block, 进程处于退出状态, task处于停止或跟踪状态无需信号 
    3. 如果信号为SIGKILL, task必须接收该信号, 寻找可杀死的线程, 进行传递
    4. 如果task运行在当前cpu上或task无信号挂起也接收信号 

# 信号回调  （发生在进程抢断的时候??）
## 前情摘要
信号处理涉及内核最底层代码, 需了解芯片架构在内各类知识, 相对晦涩难懂.
一般对现代芯片而言当进程访问一个非法地址后MMU会修改寄存器引起内核进入异常, 在异常处理时内核会分辨非法地址产生的原因(是真的非法地址还是没有映射页表)并作出不同处理. 对于处理失败的情况内核在异常处理结束时会向引起异常的task发送SIGSEGV, 在异常结束后执行调度时会首先判断该task是否有挂起信号, 如果存在则执行信号处理. 信号处理的复杂之处主要在于内核需要调用用户态程序并在程序结束后恢复内核现场
+ 也就是说入口有两个
    1. 进程触发某些异常, 这些异常跳转到内核回调中, 可以转化为信号处理类型时, 则跳转到进程的信号回调 (可能是默认的 abort)
    2. 进程调度时, 进入
## x86 堆栈
      do_work_pending(defined in arch/arm/kernel/signal.c)的作用是判断是否需要调度或信号处理;
汇编.S
x86: do_signal 作用是处理挂起信号, 保存内核寄存器状态, 为内核执行用户态回调做准备.
x86: handle_signal
x86:  setup_rt_frame
x86:  __setup_rt_frame,  其中 regs->ip = (unsigned long) ksig->ka.sa.sa_handler; 这个是信号回调