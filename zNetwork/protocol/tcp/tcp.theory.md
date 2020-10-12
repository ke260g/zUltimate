# tcp 三次握手 半连接队列 和 全连接队列
参考 https://www.cnblogs.com/xiaolincoding/p/12995358.html
1. 半连接队列，也称 syn 队列
2. 全连接队列，也称 accepet 队列 (ss -tnp | grep SYN_RECV)

## 调试
1. 查看半连接队列
    1. `ss -tnp state syn-recv`
    2. `netstat -tnp | grep SYN_RECV`
2. 查看全连接队列 (listen 的 fd 接收缓存)
    1. ` ss -tnpl` 的 Recv-Q 列
    2. `netstat -tpnl` 的 Recv-Q 列
3. 检查半连接队列溢出 `ss -s`
4. 检查全连接队列溢出 `ss -s`
5. 调整半连接队列 `tcp_max_syn_backlog`
6. 调整全连接队列 `/proc/sys/net/core/somaxconn` 和 `listen`的第二个参数
 
## 机制
1. 服务端收到客户端的 syn 请求后
    1. 内核创建 半连接对象; 加入 半连接队列
    2. 内核定时向客户端发送 syn + ack; 等待客户端 的 ack
2. 服务端收到客户端的 ack 响应后
    1. 内核把连接从 半连接对象队列移除
    2. 创建全连接对象
    3. 加入全连接队列
3. 服务端用户态进程调用 accept
    1. 内核把全连接队列从 全连接队列取出
    2. 创建 socket 返回新的 fd

## 实现
1. 全连接队列大小的确定
```c++
SYSCALL_DEFINE2(listen, int, fd, int, backlog) {
    int somaxconn;
    if (sock) {
        somaxconn = sock_net(sock->sk)->core.sysctl_somaxconn;
        if ((unsigned int)backlog > somaxconn)
            backlog = somaxconn;
        err = security_socket_listen(sock, backlog);
        if (!err) 
            err = sock->ops->listen(sock, backlog); // inet_stream_ops .listen = inet_listen;
    }
}
int inet_listen(struct socket *sock, int backlog) {
    // ...
    sk->sk_max_ack_backlog = backlog;
    // ...
}
bool sk_acceptq_is_full(const struct sock *sk) {
    return sk->sk_ack_backlog > sk->sk_max_ack_backlog;
}
```
2. 半连接队列大小 (理论值 和 实际值)
```c++
// 内核版本 3.16.81
// 1. 右移法判断溢出; 即小于 2**max_qlen_log (等于 就溢出了)
// 2. 按照以下算法; 队列大小是 sysctl_max_syn_backlog 的最近2的次方值; 向上取整
// 3. 所以 sysctl_max_syn_backlog 为 1024 对应的理论大小 2048
// 4. 由于队列满了导致的半连接丢包 主要有3种情况
// 4.1 
int reqsk_queue_is_full(const struct request_sock_queue *queue) {
    return queue->listen_opt->qlen >> queue->listen_opt->max_qlen_log;
}
int reqsk_queue_alloc(struct request_sock_queue *queue, unsigned int nr_table_entries) {
    // nr_table_entries 默认传入 512
    nr_table_entries = min_t(u32, nr_table_entries, sysctl_max_syn_backlog);
    nr_table_entries = max_t(u32, nr_table_entries, 8); // 最小值是8
    // roundup 求最近的 2的次方的值; 向上取整
    // 如 f(3) = 4; f(5) = 8; f(11) = 16; f(100) = 128
    nr_table_entries = roundup_pow_of_two(nr_table_entries + 1);
    // ...
    for (lopt->max_qlen_log = 3 /** 最小值是 8 */;
         (1 << lopt->max_qlen_log) < nr_table_entries;
         lopt->max_qlen_log++);
    // ...
    queue->listen_opt = lopt;
}

int tcp_v4_conn_request(struct sock *sk, struct sk_buff *skb) {
    // 半连接队列满了
    if ((sysctl_tcp_syncookies == 2 ||
        inet_csk_reqsk_queue_is_full(sk)) && !isn) {
        want_cookie = tcp_syn_flood_action(sk, skb, "TCP");
        if (!want_cookie)
            goto drop;
    }
    // 全连接队列满了
    if (sk_acceptq_is_full(sk)) {
        NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_LISTENOVERFLOWS);
        goto drop;
    }
    // ...

    if (want_cookie) {
        // ...
    } else if (!isn) {
        if (tmp_opt.saw_tstamp &&
            tcp_death_row.sysctl_tw_recycle &&
            (dst = inet_csk_route_req(sk, &fl4, req)) != NULL &&
            fl4.daddr == saddr) {
            // ...
        }
        // 没有开启 sysctl_tcp_syncookies 时
        // sysctl_max_syn_backlog - 当前队列大小 < sysctl_max_syn_backlog 的 1/4
        // 就会丢弃三次握手
        else if (!sysctl_tcp_syncookies &&
                (sysctl_max_syn_backlog - inet_csk_reqsk_queue_len(sk) <
                (sysctl_max_syn_backlog >> 2)) &&
                !tcp_peer_is_proven(req, dst, false)) {
            // ...
            goto drop_and_release;
        }
        // ...
    }
    // ...
}
```