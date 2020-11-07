[TOC]
# 1. 三次握手问题
## Q: TCP 两次握手行不行? 为什么?

## Q: 什么是 TCP 四次握手? 有缺点分别是什么?

## Q: 半连接DOS攻击应对 or 半连接队列满了处理?
+ 问题本身是 服务端处理三层握手不过来
```sh
echo 2048 > /proc/sys/net/ipv4/tcp_max_syn_backlog # 默认是 256
echo 2048 > /proc/sys/net/core/somaxconn           # 默认是 128
echo 65536 > /proc/sys/net/core/netdev_max_backlog # 默认是 1000
echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow  # 默认不开
echo 1 > /proc/sys/net/ipv4/tcp_syncookies         # 默认不开
echo 3 > /proc/sys/net/ipv4/tcp_synack_retries     # 默认是 5
```
1. 上调半连接队列
2. 上调全连接队列 和 listen 第二参数 backlog
3. 上调网卡驱动上协议栈前的队列
4. 在 连接队列溢出时; 往客户端发 RST. (使得客户端感知)
5. 开启 tcp_syncookies 功能; 从而不启用 半连接队列, 节省资源
6.  减少 SYN+ACK 重传次数 (缩短 销毁二次握手的超时等待)

## Q: 客户端 SYN 之后的等待时间
```sh
echo 3 > /proc/sys/net/ipv4/tcp_syn_retries
```

# 2. 四次挥手问题
## Q: server 有大量 TIME_WAIT 怎么触发的? 该如何处理?
1. 系统主动 关闭大量连接; 等待2MSL产生
2. 有大量peer端异常or超时, 导致系统关闭连接, 后续等待2MSL
3. 网络异常, 主动连接方的ACK 没有被peer端收到, peer 处于 LAST_ACK 反复重发 FIN
```sh
echo 1  > /proc/sys/net/ipv4/tcp_syncookies     # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse       # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle     # 默认 0
echo 16384 > /proc/sys/net/ipv4/tcp_max_tw_buckets # 默认 65536
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout    # 默认60
echo 5  > /proc/sys/net/ipv4/tcp_orphan_retries # ??
```
1. tcp_tw_reuse    TIME_WAIT 的 sockets 可重新用于新的 连接
2. tcp_tw_recycle  TIME_WAIT 的 sockets 启用快速回收(默认是2MSL)
3. tcp_max_tw_buckets 下调 TIME_WAIT 的 sockets
    1. 一般不建议下调这个数; 而且可能下调了用处不大; 该进入的还是会进入
4. tcp_fin_timeout    加快 FIN_WAIT_2 进入 TIME_WAIT  的等待时间
    + 超过这个时间; FIN_WAIT_2 强制进入 TIME_WAIT
5. tcp_orphan_retries 加快 FIN_WAIT_1 进入 FIN_WAIT_2 的等待时间
    + 超过这个时间; FIN_WAIT_1 强制进入 FIN_WAIT_2
6. 参数 listen的fd前设置 SO_REUSEADDR
    + `setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt , sizeof(int);`

## Q: 为什么要有四次挥手? 而握手只需要三次?


## Q: TIME_WAIT 状态的目的/意义
1. 防止具有相同"四元组"的"旧"数据包被收到
    + 四元组 = 源ip 目的ip 源端口 目的端口
2. 保证"被动关闭连接方"能被正确的关闭
    1. 保证最后的 ACK 能让被动关闭方接收 (才能从 LAST_ACK 状态退出)
    2. 如果"被动关闭连接方"处于 LAST_ACK 没有收到 ACK, 会定时重发 FIN
       "主动关闭连接方" 处于 TIME_WATI 收到 FIN, 会重发 ACK
3. 经过 2MSL 这个时间，让两个方向上的数据包都被丢弃
    1. 原来连接的数据包在网络中会消失(TTL)
    2. "被动关闭连接方" LAST_ACK 状态也超时了

# 3. 滑动窗口问题
## Q: 零窗口攻击
1. tcp的流量控制: 对端的接收窗口故意设置为零  本段不能发送数据
2. 攻击者下载大文件; 且把接收窗口设置为零; 那么服务器的 FIN 报文则无法发送
3. 服务器程序尝试关闭连接；但 FIN 报文无法发送; socket 一直处于 FIN_WAIT_1
4. 解决方案: 下调 tcp_max_orphans

## Q: 空连接 tcp_keepalive*
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

## 什么是糊涂窗口综合症问题? 如何避免?

## Q: 滑动窗口大小如何确定 ?
1. 接收方的接收缓存 - 减去已接收未读取 = 窗口大小
2. 接收方在 ack 报文中通过 tcp-header-window 字段告知发送方 窗口大小

## Q: 发送窗口 和 接收窗口是否相等 ?
1. 不完全相等
2. 接收方 用户进程处理较快; 接收窗口很快变空
3. 接收方 通过 tcp-header-window 字段告知发送方更新窗口大小
4. 这个告知报文在网络中存在时延

# 4. 重传问题
## Q: 重传分为哪几种 ?
超时重传 和 快速重传
其中超时重传又分为 数据报文丢失 和 ack报文丢失
## Q: 什么是快速重传 ?
当连续收到3次重复的ack, 即判定存在丢失
## Q: 优点 和 缺点分别是什么?
+ 优点: 避免超时重传 的等待时间过长的问题
+ 缺点: 无法确认重传窗口内全部数据还是重传丢失的数据
    + 因为有可能 丢失数据 的后面数据是正常收到的
## Q: 如何避免快速重传的缺点?
开启 sack, echo 1 > /proc/net/core/ipv4/tcp_sack
使得 接收方把接收窗口的 bitmap 通过ack返回给发送端
发送从而准确地得知丢失的部分数据, 然后只重传丢失的部分数据
## Q: RTO 过大 or 过小分别会有什么问题?
1. RTO 太大, 导致超时重传需要等待过长时间, 降低传输效率
2. RTO 太小, 导致频繁重传, 加重网络负载, 甚至负循环产生过多的重传; 且消耗内核性能收发数据
## Q: RTO 如何计算 ?
动态采样 RTT, 并加权平均

# 5. 拥塞控制问题
## Q: 拥塞控制有哪几个部分组成
1. 慢启动
2. 拥塞避免
3. 拥塞发生
4. 快速回复

# 6. 混杂综合问题
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
    + tcp_tw_recycle      (加快 TIME_WAIT 状态的回收) (仅主动方)
    + tcp_timestamps      (复用 TIME_WAIT 状态的连接) (仅主动方)
3. 数据传输
    + 扩大窗口大小 tcp_window_scaling
    + 发送缓冲区大小 tcp_wmem
    + 接收缓冲区大小 tcp_rmem
    + 开启接收缓冲区调节 tcp_moderate_rcvbuf
    + 调整内存范围和设置触发阈值 tcp_mem
在高并发服务器中，为了兼顾网速与大量的并发连接，我们应当保证缓冲区的动态调整的最大值达到带宽时延积，而最小值保持默认的 4K 不变即可。而对于内存紧张的服务而言，调低默认值是提高并发的有效手段。

同时，如果这是网络 IO 型服务器，那么，调大 tcp_mem 的上限可以让 TCP 连接使用更多的系统内存，这有利于提升并发能力。需要注意的是，tcp_wmem 和 tcp_rmem 的单位是字节，而 tcp_mem 的单位是页面大小。

## Q: 为什么会存在 tcp 分包和粘包? 代码上如何避免?
分包
要发送的数据大于TCP缓冲剩余空间
待发送数据大于MSS（最大报文长度）

粘包:
要发送的数据小于TCP缓冲区，将多次写入缓冲区的数据一起发送
接收端的应用层没有及时读取缓冲区的数据
自定义通信协议的两种方式:
  定义数据包包头
  在数据包之间设置边界(即定义包头 包尾)

## Q: tcp 的状态 列举一下?
CLOSED
SYN_SENT
SYN_RCVD
ESTABLISHED
FIN_WAIT_1
FIN_WAIT_2
TIME_WAIT
CLOSING
CLOSE_WAIT
LAST_ACK
