[TOC]
```c++
struct task_struct {
	struct signal_struct *signal;
	struct sighand_struct *sighand;

	sigset_t blocked, real_blocked;
	sigset_t saved_sigmask;	/* restored if set_restore_sigmask() was used */
	struct sigpending pending;
}


// 
SYSCALL_DEFINE2(kill, pid_t, pid, int, sig)

// userspace - syscall
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
sighandler_t signal(int signum, sighandler_t handler);
int kill(pid_t pid, int sig);
```

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
      sigaddset(&pending->signal, sig); // 拉起 task_struct->sigpending 中的 这个信号flag
      complete_signal
    }

# 信号回调 
发生在进程抢断的时候??
