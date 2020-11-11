[TOC]
+ resource:
    1. man 7 signal

A child created via fork(2) inherits a copy of its parent's signal dispositions.
 During an execve(2), the dispositions of  handled  signals  are reset to the default; the dispositions of ignored signals are left unchanged.

1. default dispositions (5个)
    1. Term  干掉            Default action is to terminate the process.
    2. Ign   无动作          Default action is to ignore the signal.
    3. Core  干掉并生成core  Default action is to terminate the process and dump core (see core(5)).
    4. Stop  暂停            Default action is to stop the process.
    5. Cont  继续如果已经暂停 Default action is to continue the process if it is currently stopped.
2. `fork()` 和 `exec()`
    1. `fork()` 创建的子进程继承父母进程的 dispositions
    2. `exec()` 创建 elf 时, 重置为默认的 dispositions
    3. `clone()` 如果指定了 CLONE_SIGHAND 才会使得子进程共享父进程的信号回调
3. 信号要么被捕捉; 要么被重新设置其他default dispositions
4. 不允许被捕捉的信号: SIGSTOP SIGKILL SIGBUS
    1. 应用: 一般进程内部捕获 SIGTERM 被正常干掉; 进程间先SIGTERM礼貌性干掉对方; 如果干不掉, 那就用SIGKILL/SIGBUS强行干掉

常用的 "标准信号" 解析
SIGHUP        1       Term    Hangup detected on controlling terminal or death of controlling process
SIGINT        2       Term    终端终止进程 Ctrl-c
SIGQUIT       3       Core    Quit from keyboard
SIGILL        4       Core    Illegal Instruction
SIGABRT       6       Core    Abort signal from abort(3)
SIGFPE        8       Core    Floating-point exception
SIGKILL       9       Term    Kill signal
SIGSEGV      11       Core    Invalid memory reference
SIGPIPE      13       Term    write 到 close 的 fd; 实际上这个可以ign; 根据 write 调用的返回值进行错误处理
SIGALRM      14       Term    Timer signal from alarm(2)
SIGTERM      15       Term    Termination signal
SIGUSR1   30,10,16    Term    User-defined signal 1
SIGUSR2   31,12,17    Term    User-defined signal 2
SIGCHLD   20,17,18    Ign     子进程暂停 or 挂掉; 场景: 通过该信号监管子进程
SIGCONT   19,18,25    Cont    Continue if stopped
SIGSTOP   17,19,23    Stop    终端停止进程 Ctrl-z
SIGTSTP   18,20,24    Stop    Stop typed at terminal
SIGTTIN   21,21,26    Stop    Terminal input for background process
SIGTTOU   22,22,27    Stop    Terminal output for background process