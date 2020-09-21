https://www.linuxidc.com/Linux/2010-10/29496p2.htm 每个 ipv4/* 字段含义
https://my.oschina.net/newchaos/blog/4339323       tcp参数调优
ls -1 /proc/sys/net/ipv4/tcp_*         每个字段的含义
Documentation/networking/ip-sysctl.txt 有比较简答的解析

# tcp 参数应用
## Q: 大量 TIME-WAIT
```sh
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse    # 默认 0; TIME-WAIT socket 的fd可以被重复利用
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle  # 默认 0; TIME-WAIT socket 快速回收(默认是2MSL)
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout # 默认60; 加快 FIN_WAIT_1 到 TIME_WAIT 的定时器
```

## Q: 三次握手丢弃连接
+ 问题本身是 服务端处理三层握手不过来
1. 上调服务端二次握手后, 等待客户端三次握手的队列
2. 上调服务端listen调用的缓存队列(已经完成连接, 但是没有accept的)
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
## tcp_keepalive_probes
## tcp_keepalive_time
tcp 发送 keepalive 心跳的时长
## tcp_limit_output_bytes
## tcp_low_latency
## tcp_max_orphans
## tcp_max_syn_backlog
## tcp_max_tw_buckets
## tcp_mem
## tcp_min_snd_mss
## tcp_min_tso_segs
## tcp_moderate_rcvbuf
## tcp_mtu_probing
## tcp_no_metrics_save
## tcp_notsent_lowat
## tcp_orphan_retries
## tcp_reordering
## tcp_retrans_collapse
## tcp_rmem
## tcp_sack
## tcp_slow_start_after_idle
## tcp_stdurg
## tcp_syn_retries
## tcp_synack_retries
## tcp_syncookies
## tcp_thin_dupack
## tcp_thin_linear_timeouts
## tcp_timestamps
针对同一个IP的连接, 之后建立的连接的时间戳必须要大于之前建立连接的最后时间戳
否则将直接丢弃这个连接
## tcp_tso_win_divisor
## tcp_tw_recycle
1. TIME_WAIT 的 sockets 启用快速回收, 从而抑制 TIME_WAIT socket的数量
2. 低版本内核 服务器 开启 tcp_tw_recycle 与 tcp_timestamps(默认开启) 会有问题
    1. 如果客户端是使用 NAT 防火墙与 服务器建立连接
    2. 那么 NAT防火墙的时间戳可能是客户端之前, 经过 NAT防火墙与服务器连接会被丢弃
## tcp_tw_reuse
## tcp_window_scaling
## tcp_wmem
## tcp_workaround_signed_windows