## Q: epoll poll select 的区别

## Q: shutdown / close  的区别

## Q: tcp 的可靠连接体现在哪几点

## Q: TCP-SYN 半连接攻击 

## Q: 三层握手 server 和 client 的状态切换

## Q: 四次挥手 server 和 client 的状态切换

## Q: 二次握手行不行 为什么

## Q: server 有大量 TIME_WAIT 如何处理
```sh
echo 1  > /proc/sys/net/ipv4/tcp_syncookies  # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_reuse    # 默认 0
echo 1  > /proc/sys/net/ipv4/tcp_tw_recycle  # 默认 0
echo 30 > /proc/sys/net/ipv4/tcp_fin_timeout # 默认60
```
1. tcp_syncookies  当出现 syn 等待队列溢出时; 启用cookies 处理
2. tcp_tw_reuse    TIME_WAIT 的 sockets 可重新用于新的 连接
3. tcp_tw_recycle  TIME_WAIT 的 sockets 启用快速回收
4. tcp_fin_timeout 减少 FIN_WAIT_2 进入 TIME_WAIT  的等待时间

## Q: udp 如何实现可靠连接

## Q: tcp 滑动窗口 相关问题

## Q: tcp 拥塞管理 相关问题

## http