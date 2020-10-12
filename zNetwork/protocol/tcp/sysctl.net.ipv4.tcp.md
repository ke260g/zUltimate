https://www.linuxidc.com/Linux/2010-10/29496p2.htm ip和tcp参数调优
https://my.oschina.net/newchaos/blog/4339323       tcp参数调优
ls -1 /proc/sys/net/ipv4/tcp_*         每个字段的含义(主要是tcp的)
Documentation/networking/ip-sysctl.txt 内核官方的解析
区分 tcp_orphan_retries tcp_retries1 tcp_retries2 tcp_synack_retries tcp_syn_retries

# tcp 参数应用


# skb 参数列表 (部分) /proc/sys/net/core/
1. `rmem_*` `wmem_*` 是整个协议栈共用的; `tcp_rmem` `tcp_wmem` 仅针对 tcp 的 
## rmem_default (默认接收缓存) (bytes)
默认 229376 建议 256960
## rmem_max (最大接收缓存) (bytes)
默认 131071 建议 513920
## wmem_default (默认发送缓存) (bytes)
默认 229376 建议 256960
## wmem_max (最大发送缓存) (bytes)
默认 131071 建议 513920
## netdev_max_backlog (网卡上协议栈前的sbk队列)
默认 1000 建议 2000
## somaxconn (全连接队列大小)
1. 默认 128 建议 1024 or 2048
2. 实际上与 listen 调用的第二个参数比较选更小值

## optmem_max (每个socket 允许的最大缓冲区)
默认 20480 建议 81920

# tcp 参数列表 (全部) /proc/sys/net/ipv4/tcp*
## tcp_abort_on_overflow
## tcp_adv_win_scale
## tcp_allowed_congestion_control
## tcp_app_win
## tcp_autocorking
## tcp_available_congestion_control
## tcp_base_mss
## tcp_challenge_ack_limit
## tcp_congestion_control
## tcp_dma_copybreak
## tcp_dsack
## tcp_early_retrans
## tcp_ecn
## tcp_fastopen
## tcp_fin_timeout
减少 FIN_WAIT_2 进入 TIME_WAIT  的等待时间
## tcp_frto


## tcp_keepalive_time
1. tcp 发送 keepalive 心跳的时长; 单位是秒
    + 当keepalive打开的情况下 TCP 发送 keepalive 消息的频率
2. 默认 7200 (即 2小时) 建议下调为 1800 (30分钟)
3. 下调该值的场景:

## tcp_keepalive_intvl
1. 当 tcp_keepalive_time 间隔后 发送的 keepalive 没有确认
   重新发送探测的等待时间
2. 默认为 75 (秒) 建议下调为 30
## tcp_keepalive_probes
1. 判定连接失效之前，发送 keepalive 的个数
2. 默认是 9（个)  建议下调为 3

## tcp_limit_output_bytes
## tcp_low_latency
## tcp_max_orphans
系 统中最多有多少个TCP套接字不被关联到任何一个用户文件句柄上。
如果超过这个数字，孤儿连接将即刻被复位并打印出警告信息
## tcp_max_syn_backlog
记 录的那些尚未收到客户端确认信息的连接请求的最大值。
对于有128M内存的系统而言，缺省值是1024，小内存的系统则是128。
如果服务器不堪重负，试 试提高这个值。
注意！如果你设置这个值大于1024，最好同时调整include/net/tcp.h中的TCP_SYNQ_HSIZE，
以保证 TCP_SYNQ_HSIZE*16 ≤ tcp_max_syn_backlo，然后重新编译内核。
## tcp_max_tw_buckets
系统同时保持timewait套接字的最大数量。如果超过这个数字，time-wait套接字将立刻被清除并打印警告信息。
## tcp_mem
1. 有三个值; 单位是内存页个数(即4K)
    1. 第一个: 最小内存使用
    2. 第二个: 触发"内存压力模式"的阈值
    3. 第三个: 内存使用的上限
2. 默认 137322 183097 274644

## tcp_min_snd_mss
## tcp_min_tso_segs
## tcp_moderate_rcvbuf
## tcp_mtu_probing
## tcp_no_metrics_save
## tcp_notsent_lowat
## tcp_orphan_retries
本端试图关闭TCP连接之前重试多少次。缺省值是7，相当于50秒~16分钟(取决于RTO)。
如果你的机器是一个重载的WEB服务器，你应该考虑减低这个值，
因为这样的套接字会消耗很多重要的资源。参见 tcp_max_orphans
## tcp_reordering
## tcp_retrans_collapse
## tcp_rmem ( { 最小 / 默认 / 最大 } 接收缓存 )
1. 有三个值; 单位是bytes
    1. 第一个 tcp 最小接收缓存
    2. 第二个 tcp 默认接收缓存 (小于 rmem_default)
    3. 第三个 tcp 最大接收缓存 (小于 rmem_max)
2. 默认 4096 87380  4011232
3. 建议 8760 256960 4088000

## tcp_sack (selected ack)
1. 选择性应答乱序接收到的报文来提高性能
    1. 通知发送者只发送丢失的报文; 而不是一整个发送窗口的??
2. 代价是消耗更多CPU计算资源
3. 默认1 表示开启 公网环境中应该开启; 内网环境中应该??

## tcp_fack
1. 转发性应答 (有什么用?)
2. 默认1 表示开启

## tcp_slow_start_after_idle (周期性重置TCP拥塞窗口) (默认是1)
1. 开启后; TCP拥塞窗口会在一个RTO时间空闲之后重置为初始拥塞窗口(CWND)大小
```c++
#define TCP_INIT_CWND		10 // 默认拥塞窗口大小

// 内核源码
static void tcp_event_data_sent(struct tcp_sock *tp, struct sock *sk) {
	struct inet_connection_sock *icsk = inet_csk(sk);
	const u32 now = tcp_time_stamp;
	const struct dst_entry *dst = __sk_dst_get(sk);

	if (sysctl_tcp_slow_start_after_idle &&
	    (!tp->packets_out && (s32)(now - tp->lsndtime) > icsk->icsk_rto))
		tcp_cwnd_restart(sk, __sk_dst_get(sk));
    // ...
}
```
2. 在广域网连接中; 网络情况时刻在变化; 定时重置拥塞窗口有效利用资源
3. 在本地局域网中; 网络情况基本稳定(比如机房内的服务器中); 为了提高传输速率可以 使用长连接并关掉该选项

## tcp_stdurg

## tcp_syn_retries: 第一次握手重传 (默认是5; 建议下调至3)
1. 直接影响 `connect()` 发送了syn但超时没有收到第二次握手后 重传的次数
2. 最终影响 `connect()` 超时时间; 默认是 63 秒(63 = 2**6 - 1); 很慢
3. `connect()` 发送 syn后等待时间不是常数; 而是根据本次调用的重传次数线性增加
    1. 首次调用; 不重传; 等待 1s
    2. 第一次重传后 等待 2  s
    3. 第二次重传后 等待 4  s
    4. 第三次重传后 等待 8  s
    5. 第四次重传后 等待 16 s
    6. 第五次重传后 等待 32 s
4. 因此; `connect()` 超时时间 = `2**(tcp_syn_retries+1) - 1`
5. 用户进程的fd 设置局部的超时时间or重传次数 tcp_syn_retries
    1. `int syn_retries = 4;`
    2. `setsockopt(sock, IPPROTO_TCP, TCP_SYNCNT, &syn_retries, sizeof(syn_retries));`
6. 用户进程的fd 实现timeout形式的 `connect()` => `connect_timeout()`
    1. fd 设置为非阻塞 `unsigned long blocked = 1; ioctl(connect_fd, FIONBIO, &blocked);`
    2. `connect()` 如果正常连接则返回
    3. `connect()` 如果连接失败; 如果 `errno == EINPROGRESS` 进入阻塞等待模式; 否则连接错误
    4. `select()`  传入 fd 监听"写事件"(参数3) 和 `struct timeval` 表示超时(参数5)
    5. `select()`  返回-1表示错误; 返回0表示超时; 返回1(只传入了一个fd)表示连接成功
    6. 连接成功后该函数需要重新把fd 设置为阻塞; 避免上层调用者误解
    7. `select()`  返回-1时; 获取底层错误原因 `getsockopt(conn_fd, SOL_SOCKET, SO_ERROR, sock_errno, sizeof(sock_errno));`

## tcp_synack_retries


## tcp_syncookies
https://segmentfault.com/a/1190000019292140 原理
https://blog.csdn.net/sinat_20184565/article/details/104828782 实现
https://lwn.net/Articles/277146/ 设计
1. 设计背景: 避免 or 减缓 syn-flood 半连接攻击
2. 实现本质: 启用后; 第二次握手时不使用backlog 半连接队列
    1. 减少 backlog 半连接队列的资源分配; 还是要分配的 不过只是一个整形
    2. hash 客户端的报文 timestamp + 客户端MSS的低3bit + T(本地时间戳)
3. 功能缺陷
    1. 客户端的MSS有8bit; 这里只用了 3bit
    2. 忽略的客户端其他字段 wscale / sack
4. 参数
    1. 数值0 表示始终不用
    2. 数值1 表示连接压力较大时启用
    3. 数值2 表示始终启用
5. 实现
```c++
tcp_conn_request()
    > cookie_init_sequence
    > cookie_v4_init_sequence
    > __cookie_v4_init_sequence
    > secure_tcp_syn_cookie
```

## tcp_thin_dupack
## tcp_thin_linear_timeouts
## tcp_timestamps
针对同一个IP的连接, 之后建立的连接的时间戳必须要大于之前建立连接的最后时间戳
否则将直接丢弃这个连接

时间戳可以避免序列号的卷绕。一个1Gbps的链路肯定会遇到以前用过的序列号。
时间戳能够让内核接受这种“异常”的数据包。
## tcp_tso_win_divisor
## tcp_tw_recycle
1. TIME_WAIT 的 sockets 启用快速回收, 从而抑制 TIME_WAIT socket的数量
2. 低版本内核 服务器 开启 tcp_tw_recycle 与 tcp_timestamps(默认开启) 会有问题
    1. 如果客户端是使用 NAT 防火墙与 服务器建立连接
    2. 那么 NAT防火墙的时间戳可能是客户端之前, 经过 NAT防火墙与服务器连接会被丢弃
## tcp_tw_reuse
1. TIME_WAIT 的 sockets 可重新用于新的连接 (如果fd满了的情况下)

## tcp_window_scaling
一般来说TCP/IP允许窗口尺寸达到65535字节。对于速度确实很高的网络而言这个值可能还是太小。这个选项允许设置上G字节的窗口大小，有利于在带宽*延迟很大的环境中使用。

一旦内核认为它无法发包，就会丢弃这个包，并向发包的主机发送ICMP通知
## tcp_wmem ( { 最小 / 默认 / 最大 } 发送缓存 )
1. 有三个值; 单位是bytes
    1. 第一个 tcp 最小发送缓存
    2. 第二个 tcp 默认发送缓存 (小于 wmem_default)
    3. 第三个 tcp 最大发送缓存 (小于 wmem_max)
2. 默认 4096 87380  4011232
3. 建议 8760 256960 4088000
## tcp_workaround_signed_windows

## tcp_retries2:   数据报文重传  (默认是15; 建议下调至5)
1. 直接影响 数据报文的重传   (为啥会影响 `recv族` ?)
2. 最终影响 `recv族` 和 `read` 的超时时间; (超时计算不懂)
    1.  5 超时时间为30秒
    2. 15 超时时间为1000秒
3. 用户进程的超时时间优先级更大
4. 在用户进程"超长时间等待"读请求时; 对端物理宕机; 则该请求会阻塞很久
    1. 如果此类请求没有与其他一般请求隔离; 那么该请求会影响一整个业务系统
    2. 针对这个问题; 设计"超长时间等待"的数据请求时需要进行隔离(线程 or 进程)
5. 实际应用中; 偶发的"超长时间等待"是存在的; 比如定时同步一个较大的数据表
6. 这种其中也反映了一个问题: 进程挂掉 vs 机器挂掉
    1. server端进程挂掉 内核会完成tcp协议层的销毁逻辑 发送RST / FIN 给client
    2. server端机器挂掉; client 端的部分连接会出现在不同形式的阻塞
    3. 如 读请求, syn重传, 已经establish的连接, last_ack等待和fin重传, fin_wait_2等待进入time_wait 


# note
有5个不同的 retries, 傻傻分不清
## tcp_retries1

## tcp_orphan_retries

## tcp_synack_retries
为了打开对端的连接，内核需要发送一个SYN并附带一个回应前面一个SYN的ACK。
也就是所谓三次握手中的第二次握手。
这个设置决定了内核放弃连接之前发送SYN+ACK包的数量。
即处于 SYN_RCVD 状态的服务端, 等待 第三次握手的 ack 的重传次数




