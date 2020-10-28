# tcp 三次握手 半连接队列 和 全连接队列
参考 https://www.cnblogs.com/xiaolincoding/p/12995358.html
1. 半连接队列, 也称 syn 队列 ( `ss -tnpl` 的 Recv-Q 列)
2. 全连接队列, 也称 accepet 队列 (`ss -tnp state syn-recv`)

## 调试
1. 查看半连接队列
    1. `ss -tnp state syn-recv`
    2. `netstat -tnp | grep SYN_RECV`
2. 查看全连接队列 (listen 的 fd 接收缓存)
    1. ` ss -tnpl` 的 Recv-Q 列
    2. `netstat -tpnl` 的 Recv-Q 列
3. 检查半连接队列溢出 `ss -s | grep ?`
4. 检查全连接队列溢出 `ss -s | grep ?`
5. 调整半连接队列 `tcp_max_syn_backlog`
6. 调整全连接队列 `/proc/sys/net/core/somaxconn` 和 `listen`的第二个参数
7. 模拟半连接溢出 `hping3 -S -p 8080 --flood 127.0.0.1` (也是模拟 http 的半连接攻击)
    + `-S` 使用 SYN 攻击
    + `-p 8080` http 服务器端口
8. 模拟全连接溢出 `wrk -t 4 -c 30000 -d 120s http:/127.0.0.1:8080`
    + `-t 4` 使用的线程数
    + `-c 30000` 半连接个数
    + `-d 120s` 持续120秒
 
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
2. 半连接队列大小的理论值为 3者最小
    1. 半连接队列理论大小 ( sysctl_max_syn_backlog 的最近2次方值向上取整 )
    2. 全连接队列大小
    3. 没有开启 tcp_syncookies 时; 则选择 sysctl_max_syn_backlog * 3/4
```c++
// 内核版本 3.16.81
// 1. 右移法判断溢出; 即小于 2**max_qlen_log (等于 就溢出了)
// 2. 按照以下算法; 队列大小是 sysctl_max_syn_backlog 的最近2的次方值; 向上取整
// 3. 所以 sysctl_max_syn_backlog 为 1024 对应的理论大小 2048
// 4. 由于队列满了导致的半连接丢包 主要有3种情况
// 4.1 半连接队列满了
// 4.2 全连接队列满了
// 4.3 没有开启 syncookies 时; 超过 sysctl_max_syn_backlog 的 3/4
//     即开启 tcp_syncookies 能扩容半连接队列(上线 仍然是 理论大小, 也受全连接队列限制)
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

## 三次握手 tcp_v4_syn_recv_sock
```c++
struct sock *tcp_v4_syn_recv_sock(struct sock *sk, struct sk_buff *skb,
				  struct request_sock *req,
				  struct dst_entry *dst){
    tcp_sync_mss(newsk, dst_mtu(dst)); // 初始化的 mss 就是目的 interface 的 mtu

}

struct sock *tcp_check_req(struct sock *sk, struct sk_buff *skb,
			   struct request_sock *req,
			   struct request_sock **prev,
			   bool fastopen) {
	child = inet_csk(sk)->icsk_af_ops->syn_recv_sock(sk, skb, req, NULL);
	if (child == NULL)
		goto listen_overflow;

	inet_csk_reqsk_queue_unlink(sk, req, prev);
	inet_csk_reqsk_queue_removed(sk, req);

	inet_csk_reqsk_queue_add(sk, req, child);
}

void inet_csk_reqsk_queue_added(struct sock *sk, const unsigned long timeout) {
    // 入列 icsk_accept_queue
	if (reqsk_queue_added(&inet_csk(sk)->icsk_accept_queue) == 0)
		inet_csk_reset_keepalive_timer(sk, timeout);
}

int tcp_rcv_state_process(struct sock *sk, struct sk_buff *skb,
			  const struct tcphdr *th, unsigned int len) {
    if (tcp_check_req(sk, skb, req, NULL, true) == NULL)
        goto discard; 
    switch (sk->sk_state) {
        case TCP_ESTABLISHED:
            tcp_data_queue(sk, skb);
            queued = 1;
            break;
        }
    }
}

static void tcp_data_queue(struct sock *sk, struct sk_buff *skb) {
    if (eaten <= 0) {
queue_and_out:
        if (eaten < 0 &&
            tcp_try_rmem_schedule(sk, skb, skb->truesize))
            goto drop;

        eaten = tcp_queue_rcv(sk, skb, 0, &fragstolen);
    }
}
```

## 一次握手 tcp_v4_conn_request
```c++
// 收到首次握手
tcp_v4_conn_request() {
    // 准许入列的校验
    // 包括队列是否满了, tcp 一些option的正确性 等

    // 开了 fastopen 且cookie 有效; 走fastopen 逻辑
	fastopen = !want_cookie && 
		   tcp_try_fastopen(sk, skb, req, &foc, dst);
    // 发送 二次握手报文
	err = tcp_v4_send_synack(sk, dst, req,
				 skb_get_queue_mapping(skb), &foc);
    // 没开 fastopen; 则进入半连接队列
	if (!fastopen) {
        // ...
		inet_csk_reqsk_queue_hash_add(sk, req, TCP_TIMEOUT_INIT);
	}
}
// 进入半连接队列 步骤1
void inet_csk_reqsk_queue_hash_add(struct sock *sk, struct request_sock *req, unsigned long timeout) {
	struct inet_connection_sock *icsk = inet_csk(sk);
	struct listen_sock *lopt = icsk->icsk_accept_queue.listen_opt;
    // 求得hash值
	const u32 h = inet_synq_hash(inet_rsk(req)->ir_rmt_addr,
				     inet_rsk(req)->ir_rmt_port,
				     lopt->hash_rnd, lopt->nr_table_entries);
    // 插入hash表
	reqsk_queue_hash_req(&icsk->icsk_accept_queue, h, req, timeout);
    // 进入半连接队列
	inet_csk_reqsk_queue_added(sk, timeout);
}
// 进入半连接队列 步骤2
void inet_csk_reqsk_queue_added(struct sock *sk, const unsigned long timeout) {
    // 入列 icsk_accept_queue
	if (reqsk_queue_added(&inet_csk(sk)->icsk_accept_queue) == 0)
		inet_csk_reset_keepalive_timer(sk, timeout);
}
```

## 半连接队列 全连接队列的数据定义
```c++
// 半连接队列只放在 inet_connection_sock 的 isck_accept_queue 中
// 实际上, 全连接socket, 半连接socket都会放在 icsk_accept_queue 中
struct inet_connection_sock {
	/* inet_sock has to be the first member! */
	struct inet_sock	  icsk_inet;
	struct request_sock_queue icsk_accept_queue;
}

// 而全连接队列多出一个副本放在 sock 的接收队列里面
struct sock {
	struct sock_common	__sk_common;
    struct sk_buff_head	sk_receive_queue;
}
```