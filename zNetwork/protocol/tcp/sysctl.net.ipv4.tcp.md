https://www.linuxidc.com/Linux/2010-10/29496p2.htm ip和tcp参数调优
https://my.oschina.net/newchaos/blog/4339323       tcp参数调优
ls -1 /proc/sys/net/ipv4/tcp_*         每个字段的含义(主要是tcp的)
Documentation/networking/ip-sysctl.txt 内核官方的解析
区分 tcp_orphan_retries tcp_retries1 tcp_retries2 tcp_synack_retries tcp_syn_retries

# tcp 参数应用
## Q: 三次握手丢弃连接
+ 问题本身是 服务端处理三层握手不过来
1. 上调服务端二次握手后, 等待客户端三次握手的队列
    + /proc/sys/net/ipv4/tcp_max_syn_backlog 
2. 上调服务端listen调用的缓存队列(已经完成连接, 但是没有accept的)
    + /proc/sys/net/core/somaxconn
2. 在 连接队列溢出时; 往客户端发 RST. (使得客户端感知)
    + 如果不发送; 客户端二次握手后处于ESTABLISHED状态; 要等48 hours触发keepavlive机制才会回收
    + 浪费客户端资源
    + 客户端connect之后马上返回用户态; 内核接收RST后会关闭连接; 此时客户端 write/read 会返回 EIO
```sh
echo 1024 > /proc/sys/net/ipv4/tcp_max_syn_backlog # 默认是 128
echo 1024 > /proc/sys/net/core/somaxconn           # 默认是 128
echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow  # 默认不开
```

## Q: 客户端 SYN 之后的等待时间
```sh
echo 3 > /proc/sys/net/ipv4/tcp_syn_retries
```

# tcp 参数列表
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
## tcp_fack
## tcp_fastopen
## tcp_fin_timeout
减少 FIN_WAIT_2 进入 TIME_WAIT  的等待时间
## tcp_frto
## tcp_keepalive_intvl
当探测没有确认时，重新发送探测的频度。缺省是75秒。
## tcp_keepalive_probes
在认定连接失效之前，发送多少个TCP的keepalive探测包。缺省值是9。
这个值乘以tcp_keepalive_intvl之后决定了，一个连接发送了keepalive之后可以有多少时间没有回应。
## tcp_keepalive_time
tcp 发送 keepalive 心跳的时长; 默认2小时
当keepalive打开的情况下，TCP发送keepalive消息的频率。
(由于目前网络攻击等因素,造成了利用这个进行的攻击很频繁,曾经也有cu 的朋友提到过,
  说如果2边建立了连接,然后不发送任何数据或者rst/fin消息,
  那么持续的时间是不是就是2小时,空连接攻击? 
  tcp_keepalive_time就是预防此情形的.我个人在做nat服务的时候的修改值为1800秒)
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
系 统同时保持timewait套接字的最大数量。如果超过这个数字，time-wait套接字将立刻被清除并打印警告信息。
## tcp_mem
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
## tcp_rmem
## tcp_sack (selected ack)
特别针对丢失的数据包使用选择性ACK，这样有助于快速恢复。
## tcp_slow_start_after_idle
## tcp_stdurg
## tcp_syn_retries
对于一个新建连接，内核要发送多少个 SYN 连接请求才决定放弃。
不应该大于255，默认值是5，对应于180秒左右时间。
(对于大负载而物理通信良好的网络而言,这个值偏高,可修改为2.这个值仅仅是针对对外的连接,
 对进来的连接,是由 tcp_synack_retries 决定的)

这个参数值得是client发送SYN如果server端不回复的话，重传SYN的次数。
对我们的直接影响呢就是connet建立连接时的超时时间。
在Linux里面默认设置是5,下面给出建议值3和默认值5之间的超时时间。

| tcp_syn_retries | timeout |
| :-: | :-: |
| 1 | min(syscall_send_timeout, 3s)  |
| 2 | min(syscall_send_timeout, 7s)  |
| 3 | min(syscall_send_timeout, 15s) |
| 4 | min(syscall_send_timeout, 31s) |
| 5 | min(syscall_send_timeout, 63s) |

## tcp_synack_retries
为了打开对端的连接，内核需要发送一个SYN并附带一个回应前面一个SYN的ACK。
也就是所谓三次握手中的第二次握手。
这个设置决定了内核放弃连接之前发送SYN+ACK包的数量。
即处于 SYN_RCVD 状态的服务端, 等待 第三次握手的 ack 的重传次数


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
## tcp_wmem
## tcp_workaround_signed_windows


# note
有几个不同的 retries, 傻傻分不清
tcp_orphan_retries
tcp_retries1
tcp_retries2
tcp_synack_retries
tcp_syn_retries



