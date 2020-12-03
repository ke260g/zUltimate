[TOC]
分析 /proc/$pid/ 目录下文件的功能
`/proc/[pid]/cmdline` 进程 `exec()` 调用的参数; 首个参数可能是相对路径or软链接
`/proc/[pid]/comm`    进程 执行的文件名, 即 `cat /proc/[pid]/exe | basename`
`/proc/[pid]/cwd`     符号链接; 指向 `getcwd()`
`/proc/[pid]/exe`     符号链接; 指向 `exec()` 可执行文件的绝对路径
`/proc/[pid]/maps`    进程页表; `task_struct->mm` 对象; 包括 名称/偏移/权限
`/proc/[pid]/fd/`     fd描述表; `task_struct->files` 对象; 
                      每个都是软链接; 指向 filepath/pipe/socket; 还有 inode 信息
`/proc/[pid]/fdinfo/` fd信息表; 每个都是文件; 指示每个fd的 pos 和 `open()` 的 flags
`/proc/[pid]/stat`    其他信息; 一连串的意义不明数字; 每列的说明要看内核
`/proc/[pid]/statm`   内存使用; 一连串的意义不明数字; 每列的说明要看内核
`/proc/[pid]/status`  stat 和 statm 的带意义说明版本
`/proc/[pid]/wchan`   当前系统调用名称
`/proc/[pid]/syscall` 当前系统调用id, 地址, 已经参数, 名称需要pmaps 工具查看
`/proc/[pid]/coredump_filter`
`/proc/[pid]/net/`    实际上, 等同于全局的 `/proc/net/` 不知道为啥这里没有用软连接
`/proc/[pid]/environ` 该进程, 进程fork的时候, 继承的环境变量

## /proc/[pid]/limits
有4列; 分别是 限制项, 软限制, 硬限制, 单位    (软限制可以上调到硬限制; 硬限制进程不能调)
Limit                     Soft Limit           Hard Limit           Units
Max cpu time              cpu 时间(总的时间片)
Max file size             最大文件大小
Max data size             数据个数
Max stack size            栈大小 
Max core file size        core大小
Max resident set          ??
Max processes             ??
Max open files            打开文件个数(只包含 打开文件) 
Max locked memory         65536                65536                bytes
Max address space         寻址空间
Max file locks            文件锁
Max pending signals       未决信号个数
Max msgqueue size         消息队列大小 (单位: 数据bytes)
Max nice priority         最大的nice 值
Max realtime priority
Max realtime timeout

## /proc/[pid]/stack 进程的内核调用栈
1. 例子1
    cat /proc/1/stack
    [<0>] ep_poll+0x400/0x450
    [<0>] do_epoll_wait+0xb8/0xd0
    [<0>] __x64_sys_epoll_wait+0x1e/0x30
    [<0>] do_syscall_64+0x57/0x190
    [<0>] entry_SYSCALL_64_after_hwframe+0x44/0xa9
2. 例子2
    cat /proc/$(pidof a.out)/stack
    [<0>] exit_to_usermode_loop+0x8f/0x160
    [<0>] prepare_exit_to_usermode+0x7e/0xa0
    [<0>] retint_user+0x8/0x8
    ```c++
    int main(int argc, char *argv[]) {
        int i = 0;
        while (1)
            i++;
        return 0;
    }
    ```

## /proc/[pid]/status 详细说明
Name: 进程/线程 名称; 可以用 prctl 修改; 默认是 `/proc/[pid]/comm`
State: 进程状态; { R:Running S:Sleeping D:Disk Z:Zombie T:Stopped }
Tgid:  线程组id, 主线程的tgid 就是 pid; pthread_create 出来的同于主线程的pid (同一进程的各线程的 tgid 相同)
Pid:   `getpid()`  进程id
PPid:  `getppid()` 父母进程id
Threads:  线程组的线程数
SigQ:  信号队列; 如 1/1724, 分母 = 未决信号的最大值