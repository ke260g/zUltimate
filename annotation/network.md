## close 和 shutdown 的区别
https://blog.csdn.net/justlinux2010/article/details/20913755
1. close 是文件系统层的关闭
    + 单个进程close一个fd后不保证马上释放连接
    + 当fd在内核中的引用数为0才会释放连接
2. shutdown 直接对socket层的关闭 (不处理引用计数) (不能shutdown非socket的fd)
    + 调用后; 其他进程占有这个连接的fd; read返回EOF; write可能触发SIGPIPE(buffer满了)
    + 对于处于 TCP_CLOSE 状态的socket 会返回 ENOTCONN错误
3. shutdown 可以选择性 只关闭读 or 只关闭写 or 同时关闭读写
    + 但 close 必须同时关闭读写方向
4. shutdown只关闭socket不释放fd; 所以进程也要调用close释放fd
    + 但 close 释放fd 释放socket

## Q: epoll poll select 的区别
https://blog.csdn.net/qq_33436509/article/details/81946968
1. fd个数; select 最大1024; epoll 和 poll 没有限制
    + 怎么修改??
2. fd拷贝到内核: poll 和 select 每次等待都要把fd集合拷贝到内核
    + epoll 则在 epoll_ctrl 一次拷贝; epoll_wait多次等待
3. fd传入(要监听的)与传出(已发生的)事件是否分离:
    1. select 没有分离; 传入传出事件共用一个变量 fd_set; 下一次监听需要重新赋值
    2. poll   事件分离；传入pollfd->events要监听的; 传出pollfd->revents已发生的
    3. epoll_wait 已分离; 不需要再次传递fd; 传出的就是有事件的fd
4. fd 是否有事件
    1. select 每次返回都要遍历一整个数组 检查fd_set 是否有事件发生
    2. poll   每次返回都要遍历一整个数组 检查revents是否有事件发生
    3. epoll  返回的就是有事件的fd集合 不需要做额外的检查

## Q: shutdown 关闭读写方向的区别
1. shutdown(SHUT_RD) 读方向
    1. 丢弃接收缓冲区的数据；后续再收到新的数据，会对数据进行 ACK，然后悄悄地丢弃
    2. 即对端还是会接收到 ACK，根本不知道数据已经被丢弃
    + 因为这样对端连接才能维持, 然后本端才能继续发送, 写方向没有关闭的
2. shutdown(SHUT_WR) 写方向 称为"半关闭连接"
    1. 发送发送缓存区的数据; 并发送一个 FIN 报文给对端; 本地进程write/send 将报错
3. shutdown(SHUT_RDWR)：相当于 SHUT_RD 和 SHUT_WR 操作各一次，关闭读和写两个方向

## Q: tcp 的可靠连接体现在哪几点

## Q: TCP-SYN 半连接攻击 

## Q: 三层握手 server 和 client 的状态切换

## Q: 四次挥手 server 和 client 的状态切换
1. 主动发起断开
    1. 被动段: CLOSE_WAIT -> LAST_ACK -> CLOSED
    2. 发起端: FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
    3. 发起端2: FIN_WAIT_1 -> TIME_WAIT -> CLOSED
2. 服务端和客户端同时断开
    2. FIN_WAIT_1 -> CLOSING -> TIME_WAIT -> CLOSED

## Q: 二次握手行不行 为什么

## Q: server 有大量 CLOSE_WAIT 怎么触发的? 该如何处理?
1. 客户端主动关闭连接; 服务端进程还没来得及关闭socket (或者程序BUG 不关闭了)


## Q: udp 如何实现可靠连接

## Q: tcp 滑动窗口 相关问题

## Q: tcp 拥塞管理 相关问题
滑动窗口大小的范围

## Q: tcp 的 seq 号是如何确定
随时间叠加值 + 随机(4元祖)

## Q: 一个连接有多少个参数唯一确定?
1. 协议号 tcp/ udp
2. 本端 ip
3. 本端 端口
4. 对段 ip
5. 对段 端口

## http
