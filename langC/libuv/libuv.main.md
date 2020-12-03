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
1. (x) uv_loop_init uv_run                              // 事件轮
2. (x) uv_timer_init uv_timer_start uv_timer_stop       // 定时器
3. (x) prepare idle check                               // 每次遍历事件轮都执行
    1. uv_idle_init    uv_idle_start    uv_idle_stop    // 紧接着 prepare 之前
    2. uv_prepare_init uv_prepare_start uv_prepare_stop // 紧接着 uv__io_poll 之前
    3. uv_check_init   uv_check_start   uv_check_stop   // 紧接着 uv__io_poll 之后
4. ( ) uv_tcp_init                                      // tcp 连接
    1. uv_tcp_bind uv_listen uv_accept                  // 服务端
    2. uv_tcp_connect                                   // 客户端
    3. uv_read_start uv_read_stop uv_write              // 读写
    4. uv_close uv_shutdown                             // 关闭