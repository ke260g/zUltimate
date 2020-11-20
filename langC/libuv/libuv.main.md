[TOC]
1. 源码 test/ 目录里面有使用例子
2. 源码 走读笔记主要分两个部分
    1. 功能实现 (如 loop, timer, tcp,  )
    2. 辅助工具 (如 heap 实现, queue 实现)
3. 误区 分析:
    1. 原生阻塞的操作: accept, read, write-to-disk
    2. 原生非阻塞操作: listen, send-to-socket(回调中可以直接用fd操作)

# feature
Full-featured event loop backed by epoll, kqueue, IOCP, event ports.
Asynchronous TCP and UDP sockets
Asynchronous DNS resolution
Asynchronous file and file system operations
File system events
ANSI escape code controlled TTY
IPC with socket sharing, using Unix domain sockets or named pipes (Windows)
Child processes
Thread pool
Signal handling
High resolution clock
Threading and synchronization primitives

# function (关注一些重点函数)
1. uv_loop_t — Event loop     // 事件轮
    1. uv_loop_init   // 事件轮初始化
    2. uv_run         // 遍历事件轮
2. uv_timer_t  — Timer handle // 定时器
    1. uv_timer_init
    2. uv_timer_start
    3. uv_timer_stop
3. uv_idle_t          // 每次遍历事件轮都执行; 紧接着 prepare 之前
    1. uv_idle_init
    2. uv_idle_start
    3. uv_idle_stop
4. uv_prepare_t       // 每次遍历事件轮都执行; 紧接着 io 之前
    1. uv_prepare_init
    2. uv_prepare_start
    3. uv_prepare_stop
5. uv_check_t         // 每次遍历事件轮都执行; 紧接着 io 之后
    1. uv_check_init
    2. uv_check_start
    3. uv_check_stop
6. uv_signal_t
    1. uv_signal_init
    2. uv_signal_start
    3. uv_signal_stop
7. uv_tcp_t
    1. 初始化 uv_tcp_init / uv_tcp_bind
    2. 服务端 uv_listen / uv_accept
    3. 客户端 uv_tcp_connect
    4. 读写 uv_read_start uv_read_stop uv_write
    5. 关闭 uv_shutdown / uv_close
    6. 设置 uv_tcp_nodelay / uv_tcp_keepalive