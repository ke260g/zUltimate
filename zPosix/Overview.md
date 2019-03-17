# Overview
+ 主要以 APUE 和 UNP 的目录为主线
+ 先看 进程和信号 然后再看 IPC

文件IO
标准IO
目录IO
网络IO
文件和目录属性
内存
进程
信号
IPC shm/sem/msq
线程和锁

## 文件和目录 这个坑巨大
+ 目录读写
+ stat/fstat/fstatat/lstat
+ 用户/组/权限
+ access
+ sync
+ umask
+ chmod fchmod fchmodat
+ chown fchown lchown

## 进程
+ setjmp/longjmp/getrlimit/setrlimit/getenv/environ
+ fork/vfork/exit
+ waitid wait3 wait4
+ exec族
+ system

## 信号
+ signal
+ kill/raise/abort
+ sigprocmask/sigpending/sigaction/sigsuspend/sigqueue
+ sigsetjmp/siglongjmp
+ sleep/nanosleep/clock_nanosleep