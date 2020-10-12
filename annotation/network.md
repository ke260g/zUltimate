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

## Q: shutdown / close  的区别
1. shutdown()函数可以选择关闭全双工连接的读通道或者写通道，如果两个通道同时关闭，则这个连接不能再继续通信。
2. close()函数会同时关闭全双工连接的读写通道，除了关闭连接外，还会释放套接字占用的文件描述符
3. shutdown()只会关闭连接，但是不会释放占用的文件描述符。
4. 所以即使使用了SHUT_RDWR类型调用shutdown()关闭连接的读写通道，也要close()来释放的文件描述符

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

## Q: server 有大量 TIME_WAIT 怎么触发的? 该如何处理?
1. tcp_syncookies  当出现 syn 等待队列溢出时; 启用cookies 处理
2. tcp_tw_reuse    TIME_WAIT 的 sockets 可重新用于新的 连接
3. tcp_tw_recycle  TIME_WAIT 的 sockets 启用快速回收(默认是2MSL)
4. tcp_fin_timeout 减少 FIN_WAIT_1/2 进入 TIME_WAIT  的等待时间
    + 超过这个时间; FIN_WAIT_1/2 强制进入 TIME_WAIT
5. tcp_orphan_retries
6. 参数 listen的fd前设置 SO_REUSEADDR
    + `setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt , sizeof(int);`

```sh
echo 1  > /proc/sys/net/ipv4/tcp_syncookies     # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse       # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle     # 默认 0
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout    # 默认60
echo 5  > /proc/sys/net/ipv4/tcp_orphan_retries # 
```

## Q: server 有大量 CLOSE_WAIT 怎么触发的? 该如何处理?
1. 客户端主动关闭连接; 服务端进程还没来得及关闭socket (或者程序BUG 不关闭了)


## Q: udp 如何实现可靠连接

## Q: tcp 滑动窗口 相关问题

## Q: tcp 拥塞管理 相关问题
滑动窗口大小的范围

## Q: 一个连接有多少个参数唯一确定?
1. 协议号 tcp/ udp
2. 本端 ip
3. 本端 端口
4. 对段 ip
5. 对段 端口

## http

