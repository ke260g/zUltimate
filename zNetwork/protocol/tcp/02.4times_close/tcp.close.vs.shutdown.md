# close 和 shutdown 的区别
https://blog.csdn.net/justlinux2010/article/details/20913755
## 标准术语
1. close 是文件系统层的关闭
    + 单个进程close一个fd后不保证马上释放连接
    + 当fd在内核中的引用数为0才会释放连接
2. shutdown 直接对socket层的关闭 (不处理引用计数) (不能shutdown非socket的fd)
    + 调用后; 其他进程占有这个连接的fd; read返回EOF; write可能触发SIGPIPE(buffer满了)
    + 对于处于 TCP_CLOSE 状态的socket 会返回 ENOTCONN错误
    + fork 继承了 fd 的进程, 将无法使用该 fd
3. shutdown 可以选择性 只关闭读 or 只关闭写 or 同时关闭读写
    + 但 close 必须同时关闭读写方向
4. shutdown只关闭socket不释放fd; 所以进程也要调用close释放fd
    + 但 close 释放fd 释放socket

## shutdown 实现
+ `int shutdown(int sockfd, int how);`
    1. how = SHUT_RD / SHUT_WR / SHUT_RDWR
```c++
sys_shutdown() {
    // 1. 根据fd 索引socket
    // 2. 根据socket 的 sk_common 结构体 的 协议方法 sk_prot 的 shutdown 释放
    // 3. udp 和 tcp 的 shutdown 方法都是 inet_shutdown
    sockfd_lookup_light(); 
    socket->sk->sk_prot->shutdown(sk, how);
}
inet_shutdown() {
    // 1. tcp 有 tcp_shutdown() 的额外回调
    // 2. udp  没有; 只有内存释放
}
tcp_shutdown() {
    // 1. 针对  TCP_ESTABLISHED、TCP_SYN_SENT、TCP_SYN_RECV TCP_CLOSE_WAIT
    //    调用 tcp_close_state() 状态变更
    tcp_close_state();
}
tcp_close_state() {
    // 1. 如果状态状态变更 则 tcp_send_fin()
    tcp_send_fin();
}
```

## close 实现
+ 调用链
```c++
sys_close()  // 根据 fd 类型调用释放方法
    > fd->file->filp_close();  // file 是 struct file
    > __fput()
    > fd->file->f_op->release(); // socket 的 release 函数是 sock_close
sock_close() // 根据 sk 类型调用释放方法
    > inet_release()    // tcp 和 udp
    > raw_release()     // raw
    > netlink_release() // netlink
    > unix_release()    // unix 套接字
inet_release() // 根据协议簇调用释放方法
    > tcp_close()       // struct proto tcp_prot;
    > udp_lib_close()   // struct proto udp_prot;
    > ping_close()      // struct proto ping_prot;
    > raw_close()       // struct proto raw_prot;
tcp_close();
```
