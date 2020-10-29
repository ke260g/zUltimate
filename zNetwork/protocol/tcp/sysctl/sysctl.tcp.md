https://www.linuxidc.com/Linux/2010-10/29496p2.htm ip和tcp参数调优
https://my.oschina.net/newchaos/blog/4339323       tcp参数调优
ls -1 /proc/sys/net/ipv4/tcp_*         每个字段的含义(主要是tcp的)
Documentation/networking/ip-sysctl.txt 内核官方的解析
区分 tcp_orphan_retries tcp_retries1 tcp_retries2 tcp_synack_retries tcp_syn_retries
    1. tcp_orphan_retries: LAST_ACK, FIN_WAIT_1 状态下的 fin 报文重传
    2. tcp_retries1
    3. tcp_retries2:
    4. tcp_syn_retries:    SYN_SENT 状态下的 syn 重传
    5. tcp_synack_retries: SYN_RECV 状态下的 syn-ack 重传
理解 RTO 时间
内存 rmem_default rmem_max wmem_default wmem_max
     tcp_rmem tcp_wmem tcp_mem
     内存设置过小 无法充分使用网络带宽 无法提升传输效率
     内存设置过大 滞留在缓冲区的待 ack太多 耗尽内存资源 导致无法响应新的连接

诊断tcp:
/proc/net/netstat
/proc/net/snmp
/proc/$pid/net/snmp

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
## optmem_max (协议栈最大内存)

## netdev_max_backlog (网卡上协议栈前的sbk队列)
默认 1000 建议 2000
## somaxconn (全连接队列大小)
1. 默认 128 建议 1024 or 2048
2. 实际上与 listen 调用的第二个参数比较选更小值


## optmem_max (每个socket 允许的最大缓冲区)
默认 20480 建议 81920

# ip 参数列表 (/proc/sys/net/ipv4/ip_local_port_range)

# tcp 参数列表 (全部) /proc/sys/net/ipv4/tcp*
## tcp_abort_on_overflow
1. 在 连接队列溢出时; 往客户端发 RST. (使得客户端感知)
2. 开启后客户端connect之后马上返回用户态; 内核接收RST后会关闭连接; 此时客户端 write/read 会返回 EIO
3. 如果服务器不发送RST, 服务端连接实际还没有建立, 还处于半连接队列(即没有销毁)
   处于ESTABLISHED状态下的客户端会等待服务器的ack,
   客户端等待超时后, 会重发 第三次握手的ack,
   直到服务端半连接队列有空间后, 连接仍然可以建立
4. 当服务器过载时, 建议临时开启, 通知客户端的应用层执行"停顿"的逻辑,
   避免服务器长期过载
   客户端也因此可以得知服务器半连接队列过载了

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
## tcp_fastopen 绕过三次握手
1. 参数值:
    1. 1 客户端支持 `connect()` 和 收发调用
    2. 2 服务端支持 `listen()` 和 收发调用
    3. 3 服务端和客户端都支持 (bit-or-wise 操作)
    4. 0 关闭
2. 首次建立连接的流程
    1. client send tcp-syn, tcp头包含fastopen-option, cookie字段为空, 指示客户端开启了tcp_fastopen
    2. server 接收到开启 tcp_fastopen 空 cookie 的 syn, 生成 cookie, 放到 syn-ack 种返回客户端
    3. client 接收到带 cookie 的 syn-ack; 缓存到本地
3. 后序建立连接的流程 (cookie 有效) (比如一次 http 的 get/post 请求)
    1. client 发送 { cookie + syn + data } 的报文到服务端
    2. server 检验 cookie
    3. server 返回 syn+ack, ack 包含data部分; 且服务端进程 listen + recv得到 data
    4. 服务端后序处理并返回 响应内容到客户端
4. 后序建立连接的流程 (cookie 无效)
    1. client 发送 { cookie + syn + data } 的报文到服务端
    2. server 检验 cookie
    3. server 返回 syn+ack, ack 仅仅只是 client 的 syn
    4. 客户端重发 data (窗口算法 ack 没确认); 且客户端删除 cookie 缓存
    5. 下一次客户端建立连接; 重新走首次建立连接的流程
+ 这个功能最终减少了三次握手中的一次 RTT 消耗
+ 这个功能使用了 tcp-option 的类型 34 字段

## tcp_fin_timeout
FIN_WAIT_2 进入 TIME_WAIT  的等待时间; 单位是秒
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
1. 默认关闭
2. 允许TCP/IP栈适应在高吞吐量情况下低延时的情况
如果开启, 会有啥后果?


## tcp_max_syn_backlog
记 录的那些尚未收到客户端确认信息的连接请求的最大值。
对于有128M内存的系统而言，缺省值是1024，小内存的系统则是128。
如果服务器不堪重负，试 试提高这个值。
注意！如果你设置这个值大于1024，最好同时调整include/net/tcp.h中的TCP_SYNQ_HSIZE，
以保证 TCP_SYNQ_HSIZE*16 ≤ tcp_max_syn_backlo，然后重新编译内核。
## tcp_max_tw_buckets
系统同时保持timewait套接字的最大数量。如果超过这个数字，time-wait套接字将立刻被清除并打印警告信息。
## tcp_mem (tcp总内存)
1. TIME_WAIT socket 的最大数量
2. 超过这个数值 socket 进入 TIME_WAIT 后直接销毁不再等待
    + 内核会打印告警信息
1. 有三个值; 单位是内存页个数(即4K)
    1. 第一个: 最小内存使用
    2. 第二个: 触发"内存压力模式"的阈值
    3. 第三个: 内存使用的上限
2. 默认 137322 183097 274644

## tcp_min_snd_mss
## tcp_min_tso_segs
## tcp_moderate_rcvbuf
开启发送缓冲的调节功能; 默认关闭

## tcp_mtu_probing
## tcp_no_metrics_save (路由计量)
开启后; tcp 缓存连接的多个指标到 route cache 中;
从而在短时间内 重新是相同ip建立连接时, 更快地设置初始化条件(窗口大小?? 还是什么东西??)
## tcp_notsent_lowat

## tcp_orphan_retries
1. FIN 报文的重传次数
    1. FIN_WAIT_1 状态下的 FIN 重传次数
    2. LAST_ACK   状态下的 FIN 重传次数
2. 遇到 零窗口攻击; 仅仅下调该值是不够的; 还得下调 tcp_max_orphans
3. 这个值默认为0, 内核中表示数值8 (相当于50秒~16分钟(取决于RTO)) 
    + net/ipv4/tcp_timer.c::tcp_orphan_retries 函数的逻辑: 当该值为0时 重传8次

## tcp_max_orphans
1. orphans 形式上 没有fd关联的连接
2. orphans 对应着 close 后还没完成生命周期的连接
    1. 大部分是 (FIN_WAIT_1, FIN_WAIT_2, LAST_ACK, CLOSING, TIME_WAIT)
    2. 注意 CLOSE_WAIT 不是; 该状态下客户端还没 close
    3. 注意 主动 shutdown 但没有 close 的连接不会判定为孤儿进程
3. 超过这个数值后; 新增 orphans 连接不走四次握手; 内核发送RST 然后销毁
4. 过载服务器可以下调该值
    1. 默认是 65536
    2. 建议值 32768 16384

## tcp_reordering
## tcp_retrans_collapse
## tcp_rmem ( { 最小 / 默认 / 最大 } 接收缓存 )
1. 有三个值; 单位是bytes
    1. 第一个 tcp 最小接收缓存
    2. 第二个 tcp 默认接收缓存 (小于 rmem_default)
    3. 第三个 tcp 最大接收缓存 (小于 rmem_max)
2. 默认 4096 87380  4011232
3. 建议 8760 256960 4088000
socket 的 SO_RCVBUF 参数优先级更高；覆盖 sysctl 的调节

`echo 1 > tcp_moderate_rcvbuf` 才能开启; 默认是关闭的
接收缓冲区可以根据系统空闲内存的大小来调节接收窗口：
  如果系统的空闲内存很多，就可以自动把缓冲区增大一些，从而通知发送者发送更多的报文
  如果系统的空闲内存不够, 则降低接收缓冲，保证系统正常工作
  系统内存是否空闲的判定标准 = `tcp_mem[1] <= free_mem <= tcp_mem[2]`
  即在 tcp_mem 的第二和第三参数间 表示内存不够; 触发调整逻辑



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

## tcp_syn_retries: 第一次握手重传
1. 直接影响 `connect()` 发送了syn但超时没有收到第二次握手后 重传的次数
2. 最终影响 `connect()` 超时时间; 默认是 63 秒(63 = 2**6 - 1); 很慢
3. `connect()` 发送 syn后等待时间不是常数; 而是根据本次调用的重传次数线性增加
    1. 首次调用; 不重传; 等待 1s
    2. 第一次重传后 等待 2  s
    3. 第二次重传后 等待 4  s
    4. 第 n次重传后 等待 `2**n` s
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
+ LAN 环境中, 建议下调至3, 使得错误连接错误尽快保留给客户端
+ WAN 环境中, 在网络不稳定,丢包严重时, 建议上调, 从而保证连接, 避免过多的销毁重连

## tcp_synack_retries 第二次握手重传
1. 直接影响 `listen()` 处于 SYN_RCVD 状态的fd (`ss -tnpl state syn-recv`) 的重传次数
2. 最终影响 `listen()` 处与 SYN_RCVD 状态的fd (`ss -tnpl state syn-recv`) 被销毁时间
3. 内核首次发送 syn-ack 报文后; 进入超时等待
    1. 首次调用; 不重传; 等待 1s
    2. 第一次重传后 等待 2  s
    3. 第二次重传后 等待 4  s
    4. 第 n次重传后 等待 `2**n` s
4. 超时时间 = `2**(tcp_synack_retries+1) - 1`
+ 如果服务器面对半连接攻击和重度访问, 建议下调, 加速半连接队列节点的销毁
+ 如果网络不稳定, 丢包严重时, 建议上调, 从而保证连接

## tcp_syncookies
1. 设计背景: 避免 or 减缓 syn-flood 半连接攻击
2. 实现本质: 启用后; 第二次握手时不使用 backlog 半连接队列
3. 功能缺陷
    1. 客户端的MSS有8bit; 这里只用了 3bit
    2. 忽略的客户端其他字段 wscale / sack
4. 参数
    1. 数值0 表示始终不用
    2. 数值1 表示连接压力较大时启用 (过载服务器一般用这个)
    3. 数值2 表示始终启用

## tcp_thin_dupack
## tcp_thin_linear_timeouts
## tcp_timestamps
针对同一个IP的连接, 
之后建立的连接的时间戳必须要大于之前建立连接的最后时间戳
否则将丢弃这个连接.

TCP时间戳（会在TCP包头增加12个字节），
以一种比重发超时更精确的方法（参考RFC 1323）
来启用对RTT 的计算;
在代理服务器环境中, 在系统时间对不上的情况下, 可能会导致被丢弃

tcp-option-type: 8

1. 优点:
    1. 避免 2MSL 即可以绕过 TIME_WAIT; (因为报文自带时间戳)
    2. 避免了 序列号 绕回的问题(高速网络中会遇到); (因为报文自带时间戳)
2. 缺陷:
    1. 

## tcp_tso_win_divisor
## tcp_tw_recycle
1. TIME_WAIT 的 sockets 启用快速回收, 从而抑制 TIME_WAIT socket的数量
    1. 但时有可能在 socket 复用时导致数据错乱
2. 低版本内核 服务器 开启 tcp_tw_recycle 与 tcp_timestamps(默认开启) 会有问题
    1. 如果客户端是使用 NAT 防火墙与 服务器建立连接
    2. 那么 NAT防火墙的时间戳可能是客户端之前, 经过 NAT防火墙与服务器连接会被丢弃
## tcp_tw_reuse
1. TIME_WAIT 的 sockets 可重新用于新的连接 (如果fd满了的情况下)
2. 只用于连接发起方 `connect()`
    + 被动连接方 `listen()` 是没用的
3. 安全性理论
    1. 由于只用于连接发起方; 因此客户端tcp端口短时间不冲突; 四元组  不会冲突
    2. TIME_WAIT 的 socket 至少要在1秒后复用(内核实现)；配合 tcp_timestamps 避免冲突
3. 复用流程1
    1. `connect()` 方复用 socket; 发送 syn
    2. `listen()`  方上一个连接已经脱离 LAST_ACK
    3. 连接正常建立
4. 复用流程2
    1. `connect()`端 复用 socket; 发送 syn
    2. `listen()` 端 上一个连接还处于 LAST_ACK；检查 syn 的 timestamps 后丢弃 
    3. `listen()` 端 在 LAST_ACK 状态中重复 FIN；而 `connect()`方处于 SYN_SENT
    4. `connect()`端 SYN_SENT 状态中收到 FIN 会返回 RST 给 `listen()` 端
    5. `listen()` 端 最终脱离 LAST_ACK 进入CLOSED
    6. `listen()` 端 最终收到 SYN 走正常的三次握手
```log
      connect()端                    listen() 端
       SYN_SENT @ ----- SYN -------> * LAST_ACK (timestamps 丢包)
tcp_syn_retries @ ----- SYN -------> * LAST_ACK (timestamps 丢包)
                @ .................. *
                @ <---- FIN -------- * tcp_orphan_retries
                @ ----- RST -------> * LAST_ACK
                @ .................. * CLOSED
tcp_syn_retries @ ----- SYN -------> * CLOSED
                  ..................
                  正常的三次握手过程
```

## tcp_window_scaling (扩展传输窗口)
一般来说TCP/IP允许窗口尺寸达到65535字节。
对于速度确实很高的网络而言这个值可能还是太小。
这个选项允许设置上G字节的窗口大小,
有利于在带宽*延迟很大的环境中使用。

一旦内核认为它无法发包，就会丢弃这个包，并向发包的主机发送ICMP通知

启用RFC 1323定义的window scaling，
要支持超过64KB的TCP窗口，
必须启用该值（1表示启用），
TCP窗口最大至1GB，TCP连接双方都启用时才生效。

## tcp_wmem ( { 最小 / 默认 / 最大 } 发送缓存 )
1. 有三个值; 单位是bytes
    1. 第一个 tcp 最小发送缓存
    2. 第二个 tcp 默认发送缓存 (小于 wmem_default)
    3. 第三个 tcp 最大发送缓存 (小于 wmem_max)
2. 默认 4096 87380  4011232
3. 建议 8760 256960 4088000
socket 的 SO_SNDBUF 参数优先级更高; 覆盖 sysctl 的调节

发送缓冲区是自行调节的，
当发送方发送的数据被确认后，
并且没有新的数据要发送，
就会把发送缓冲区的内存释放掉。

## tcp_workaround_signed_windows

## tcp_retries2 数据报文重传  (默认是15; 建议下调至5)
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



