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
4. tcp_fin_timeout 减少 FIN_WAIT_2 进入 TIME_WAIT  的等待时间
    + 超过这个时间; FIN_WAIT_2 强制进入 TIME_WAIT
5. tcp_orphan_retries
6. 参数 listen的fd前设置 SO_REUSEADDR
    + `setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt , sizeof(int);`

```sh
echo 1  > /proc/sys/net/ipv4/tcp_syncookies     # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse       # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle     # 默认 0
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout    # 默认60
echo 5  > /proc/sys/net/ipv4/tcp_orphan_retries # ??
```

## Q: 如何进行tcp优化 3个角度
1. 三次握手
    + 第一次 tcp_syn_retries
    + 第二次 半连接队列扩容 tcp_max_syn_backlog somaxconn backlog(listen第二参数)
    + 第二次 半连接队列停用 tcp_syncookies 一般来说只需要设置为 1
    + 第二次 半连接队列时效 tcp_synack_retries
    + 第三次 
2. 四次挥手
3. 数据传输