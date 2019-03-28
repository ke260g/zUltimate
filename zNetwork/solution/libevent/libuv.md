2 abstractions to work with, in combination with the event loop: handles and requests.

Handles represent long-lived
Requests represent (typically) short-lived operations

TCP sockets, UDP sockets, and pipes for file I/O and IPC are all treated as stream subclasses

fs 是际遇线程池 阻塞的
net 是内核原生阻塞

fs open/read/write/close 我认为应该再封装一层
pipe 应该再封装一层:
    alloc_buffer  应该有个默认的
    read-callback 应该自动回收buffer(或使用单一的一块)
    read-EOF回调,
    read-Normal 回调
    write-callback自动回收buffer     (或使用单一的一块)
    屏蔽read_req_t/write_req_t逻辑, 用底层维护
fs notification, 可递归监控目录

tcp-wrapper
listen_callback (再listen-callback中, 简化accept-callback准备流程)
accept_callback,
udp
dns 封装, 这个很棒 getaddrinfo

thread, queue(很棒), mutex/rwlock

process, spawn(fork的代动词), 发信号

uv-loop 间通讯

utilities:
timer, idle(每个事件循环后都执行一次), 
与第三方库的 I/O事件循环交互, 例子是curl
依赖注入, 检查是否缺少so
tty 事件, tmux的底层?