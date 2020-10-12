[TOC]
术语:
半连接队列: listen 中内核已发发送 syn+ack 的队列 (又被称为 syn 队列)
全连接队列: listen 中内核已完成三次握手当没有 accept 的队列 (又被称为 accept 队列)
# Q: 三次握手丢弃连接
+ 问题本身是 服务端处理三层握手不过来
```sh
echo 1024 > /proc/sys/net/core/somaxconn           # 默认是 128
echo 65536 > /proc/sys/net/core/netdev_max_backlog # 默认是 1000
echo 1024 > /proc/sys/net/ipv4/tcp_max_syn_backlog # 默认是 128
echo 1 > /proc/sys/net/ipv4/tcp_abort_on_overflow  # 默认不开
```
## 1. tcp 无关
1. 上调服务端listen调用的缓存队列(已经完成连接, 但是没有accept的)
    + /proc/sys/net/core/somaxconn (默认128; 可上调为1024)
2. 上调网卡驱动接收到的还没有送到内核协议栈的skb
    + /proc/sys/net/core/netdev_max_backlog (默认1000；可上调为65536)
## 2. tcp 相关
1. 上调服务端二次握手后, 等待客户端三次握手的队列
    + /proc/sys/net/ipv4/tcp_max_syn_backlog
2. 在 连接队列溢出时; 往客户端发 RST. (使得客户端感知)
    + 如果不发送; 客户端二次握手后处于ESTABLISHED状态; 要等48 hours触发keepavlive机制才会回收
    + 浪费客户端资源
    + 客户端connect之后马上返回用户态; 内核接收RST后会关闭连接; 此时客户端 write/read 会返回 EIO

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