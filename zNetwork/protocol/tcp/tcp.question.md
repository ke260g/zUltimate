[TOC]
术语:
半连接队列: listen 中内核已发发送 syn+ack 的队列 (又被称为 syn 队列)
全连接队列: listen 中内核已完成三次握手当没有 accept 的队列 (又被称为 accept 队列)

# Q: 三次握手丢弃连接 and  半连接DOS攻击预防
+ 问题本身是 服务端处理三层握手不过来
```sh
echo 2048 > /proc/sys/net/ipv4/tcp_max_syn_backlog # 默认是 256
echo 2048 > /proc/sys/net/core/somaxconn           # 默认是 128
echo 65536 > /proc/sys/net/core/netdev_max_backlog # 默认是 1000
echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow  # 默认不开
echo 1 > /proc/sys/net/ipv4/tcp_syncookies         # 默认不开
echo 1 > /proc/sys/net/ipv4/tcp_
```
1. 上调半连接队列
2. 上调全连接队列 和 listen 第二参数 backlog
3. 上调网卡驱动上协议栈前的队列
4. 在 连接队列溢出时; 往客户端发 RST. (使得客户端感知)
5. 开启 tcp_syncookies 功能
6.  减少 SYN+ACK 重传次数 (缩短 销毁二次握手的超时等待)

## Q: 零窗口攻击
1. tcp的流量控制: 对端的接收窗口故意设置为零  本段不能发送数据
2. 攻击者下载大文件; 且把接收窗口设置为零; 那么服务器的 FIN 报文则无法发送
3. 服务器程序尝试关闭连接；但 FIN 报文无法发送; socket 一直处于 FIN_WAIT_1
4. 解决方案: 下调 tcp_max_orphans

#  Q: 客户端 SYN 之后的等待时间
```sh
echo 3 > /proc/sys/net/ipv4/tcp_syn_retries
```

# Q: 空连接 tcp_keepalive*
1. 下调 3个 tcp_keepalive_* 参数
```sh
echo 1800 > /proc/sys/net/ipv4/tcp_keepalive_time # 默认 7200 (两小时)
echo 3 > /proc/sys/net/ipv4/tcp_keepalive_probes  # 默认 9
echo 30 > /proc/sys/net/ipv4/tcp_keepalive_invtl  # 默认 75 （秒)
```
2. tcp_keepalive 的机制:
    1. 当超过 tcp_keepalive_time 时间没有收到数据
    2. 尝试发送 tcp_keepalive_probes 次 keepalive
    3. 发送后等待间隔 tcp_keepalive_invtl
    4. 如果等待间隔中 接收到对端 keepalive 连接维持
    5. 否则超过发送次数; 内核判定连接失效 (errno ??)
3. 实际上 应用层协议也应该设计心跳逻辑
    1. 在应用层以更短的时间销毁 "无效" 连接
4. 重压服务器需要下调的场景
    1. 避免空连接攻击
    2. 加快对端物理宕机后; 本段socket的回收

## Q: server 有大量 TIME_WAIT 怎么触发的? 该如何处理?
1. tcp_syncookies  当出现 syn 等待队列溢出时; 启用cookies 处理
2. tcp_tw_reuse    TIME_WAIT 的 sockets 可重新用于新的 连接
3. tcp_tw_recycle  TIME_WAIT 的 sockets 启用快速回收(默认是2MSL)
4. tcp_max_tw_buckets 下调 TIME_WAIT 的 sockets
    1. 一般不建议下调这个数; 而且可能下调了用处不大; 该进入的还是会进入
5. tcp_fin_timeout 减少 FIN_WAIT_2 进入 TIME_WAIT  的等待时间
    + 超过这个时间; FIN_WAIT_2 强制进入 TIME_WAIT
6. tcp_orphan_retries
7. 参数 listen的fd前设置 SO_REUSEADDR
    + `setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt , sizeof(int);`

```sh
echo 1  > /proc/sys/net/ipv4/tcp_syncookies     # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse       # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle     # 默认 0
echo 16384 > /proc/sys/net/ipv4/tcp_max_tw_buckets # 默认 65536
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout    # 默认60
echo 5  > /proc/sys/net/ipv4/tcp_orphan_retries # ??
```

## Q: TIME_WAIT 状态的目的/意义
1. 防止具有相同"四元组"的"旧"数据包被收到
    + 四元组 = 源ip 目的ip 源端口 目的端口
2. 保证"被动关闭连接方"能被正确的关闭
    1. 保证最后的 ACK 能让被动关闭方接收 (才能从LAST_ACK 状态退出)
3. 经过 2MSL 这个时间，让两个方向上的数据包都被丢弃
    1. 原来连接的数据包在网络中会消失(TTL)

## Q: 如何进行tcp优化 3个角度
1. 三次握手
    + 第一次 tcp_syn_retries
    + 第二次 半连接队列扩容 tcp_max_syn_backlog somaxconn backlog
    + 第二次 半连接队列停用 tcp_syncookies 一般来说只需要设置为 1
    + 第二次 半连接队列时效 tcp_synack_retries
    + 第三次 全连接队列扩容 somaxconn backlog(listen第二参数)
    + 绕过三次握手 tcp_fastopen
2. 四次挥手
    + tcp_orphan_retries  (FIN报文重传次数) (主被动方)
    + tcp_max_orphans     (LAST_ACK/FIN_WAIT_1/FIN_WAIT_2 状态下的孤儿连接数) (主被动方)
    + tcp_fin_timeout     (FIN_WAIT_2 的状态维持时间) (仅主动方)
    + tcp_max_tw_buckets  (TIME_WAIT 状态的连接上限)  (仅主动方)
    + tcp_tw_reuse        (复用 TIME_WAIT 状态的连接) (仅主动方)
    + tcp_timestamps      (复用 TIME_WAIT 状态的连接) (仅主动方)
3. 数据传输
    + 扩大窗口大小 tcp_window_scaling
    + 发送缓冲区大小 tcp_wmem
    + 接收缓冲区大小 tcp_rmem
    + 开启接收缓冲区调节 tcp_moderate_rcvbuf
    + 调整内存范围和设置触发阈值 tcp_mem
在高并发服务器中，为了兼顾网速与大量的并发连接，我们应当保证缓冲区的动态调整的最大值达到带宽时延积，而最小值保持默认的 4K 不变即可。而对于内存紧张的服务而言，调低默认值是提高并发的有效手段。

同时，如果这是网络 IO 型服务器，那么，调大 tcp_mem 的上限可以让 TCP 连接使用更多的系统内存，这有利于提升并发能力。需要注意的是，tcp_wmem 和 tcp_rmem 的单位是字节，而 tcp_mem 的单位是页面大小。

# 糊涂窗口综合症问题