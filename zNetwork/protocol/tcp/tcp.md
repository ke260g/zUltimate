[TOC]
# TCP 可靠连接
+ 三次握手
+ 四次挥手
+ 自适应包大小 Nagle
+ 超时重传
+ 滑动窗口
+ 心跳包

# TCP加速
+ Nagle算法 等待足够多数据一起发
    + TCP_NODELAY 关掉这个算法
+ 携带ACK 等一会把 下一帧要发送的数据连着上一个ack一起发
+ 滑动窗口
+ 数据拥塞
+ 超复杂定时器
    超时定时器 坚持定时器 2MSL定时器

# 1. 三次握手
client: send SYN seq=J
(client => ) (server => SYN_RCVD)
server: send SYN ACK seq=K ack=J+1
client: send ACK ack=K+1
(client => ESTABLISHED) (server => ESTABLISHED)

# 2. 四次握手
## 2.1 四次挥手 状态流1  (FIN - FIN - ACK - ACK) (两端同时close)
```log
主动端                                         主动端
close  @ ----- FIN -------> <---- FIN -------- # close
       @ ----- ACK -------> <---- ACK -------- #
实际上:

主动端                      主动端
close  @ ----- FIN -------> #
       @ <---- FIN -------- # close
       @ ----- ACK -------> #
       @ <---- ACK -------- #
```
1. 两端: FIN_WAIT_1 -> CLOSING -> TIME_WAIT -> CLOSED
  + 当两端同时发起关闭时; 即一段发起关闭, FIN报文还没到另一端时, 另一端也发起了关闭

## 2.2 四次挥手 状态流2  (FIN - ACK - FIN - ACK)
```log
主动关闭                         被动关闭
close       @ ----- FIN -------> #
FIN_WAIT_1  @                    #
            @ <---- ACK -------- # 这个是内核收到FIN后触发的; 然后用户态EIO
FIN_WAIT_2  @                    # CLOSE_WAIT
            @ <---- FIN -------- # 用户 close / shutdown
TIME_WAIT   @                    # LAST_ACK
            @ ----- ACK -------> #
CLOSED      @                    # CLOSED     收到最后的ACK后才进入CLOSED状态
```
1. 主动端: FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
2. 被动端: CLOSE_WAIT -> LAST_ACK -> CLOSED

## 2.3 四次挥手 状态流3  (FIN - ACK&FIN - ACK)
```log
主动关闭                         被动关闭
close       @ ----- FIN -------> *
FIN_WAIT_1  @                    *
            @ <---- ACK FIN----- * 用户 close / shutdown
            @                    * CLOSE_WAIT -> LAST_ACK (短时间内切换)
            @ ----- ACK -------> *
TIME_WAIT   @                    * CLOSED     收到最后的ACK后才进入CLOSED状态
```

1. 主动端: FIN_WAIT_1 -> TIME_WAIT -> CLOSED
2. 被动端: CLOSE_WAIT -> LAST_ACK -> CLOSED
  + 被动端 内核把用户态的 ACK 和 FIN 合并到一起发了一个包

# 超时重传 ( seq + ack )
+ 三种情况 (正常 发送方的包未到达接收方 接收方的ack未到达发送方)
+ 发送方 针对长时间没收到ack的包 重发
+ 接收方 针对重复收到的已经发过ack的seq包 重新发ack
+ 发送方的**等待时长**是动态计算的
  + 首次等待 两倍 往返时长
  + 然后等待 四倍 往返时长
  + 足够多次重发后 直接断开连接
+ 每一对seq + ack 称为一个segment
+ segment大小在三次握手时 确认

# 3. 滑动窗口
https://blog.csdn.net/yao5hed/article/details/81046945
1. 一对一的 seq + ack 太慢了
2. 定义窗口 为N个segment 的大小
3. 一次性发送N个segment 等待N个ACK
    + 因此导致丢包本质有两种: 发送段包体丢了 接收端ack丢了
4. 发送端
  1. 同一窗口中的segment 即使上一段没有收到ACK 也继续发
  2. 窗口中**第**m个segment的ack收到后 即可把窗口 往后移m个segment
  3. 当**第**m个segment的ack收到后; 当作 0 ~ m-1 个segment的ack收到; 即使没收到也无需重发
  4. 避免因ack丢失 导致的冗余重发
  5. 当**第**n个segment的ack重复收到 表明该segment的下一段 丢失了
5. 接受端 (移动窗口 + 发送首个非连续处segment的ack)
  1. 同一窗口中的segment 当第i个segment没有收到 但收到了其后i+1, i+2的segment
  2. 这时 接受端 每次收到i + j的segment时, 发第i个ack
  3. 直到收到 i个segment后, 且窗口中segment完全连续时 发送最末端ack
  4. 对于收到重复的seq包 不作特别处理  沿用上面的策略
## 3.1 发送缓存 分为4类:
1. 已发送，已收到ACK    (在窗口前)
2. 已发送，未收到ACK    (在窗口中)
3. 未发送，但允许发送   (在窗口中)
4. 未发送，但不允许发送 (在窗口后)
## 3.2 接收缓存 分为3类：
1. 已接收               (在窗口前)
2. 未接收, 准备接收     (在窗口中)
3. 未接收, 不准备接收   (在窗口后)
## 3.3 发送缓存 和 接受缓存的区别
1. 发送窗口一旦收到窗口内segment任意ack；就会右移发送窗口
2. 接收窗口只有收到窗口左边界开始连续的k个segment; 才会右移接收窗口k (k >= 1)
    1. 当窗口内较前segment未接收; 但提前收到窗后内较后方的segment窗口不会移动
    2. 且不对窗口内靠后segment发送ack。而是重发左边界-1的ack。
    3. 以此确保对端会对窗口内所有segment重传。
+ 这种区别的本质是为了 同步两端的窗口左边界
## 3.4 窗口大小
1. 限制: 窗口大小不能大于序列号空间的一半
2. 目的: 为了不让两个窗口出现交迭
## 3.5 收发场景
1. 接收方和发送方窗口是左边界同步
2. 现发送方把窗口内所有 segment 发送
3. 会有如下几个场景
### 场景1: 窗口内segment 接收方全部接收; 发送方全收到最后segment的ack
1. 接收方和发送方同时把窗口右移
### 场景2: 窗口内segment 接收方全部接收; 但是ack全部丢掉了
1. 接收方全部接收后; 窗口右移
2. 发送方等待ack超时; 重发窗口内的segment
3. 接收方收到 当前接收窗口左边界之前的 segment；重发左边界-1的ack
4. 发送方收到 ack后右移窗口
### 场景3: 窗口内segment 接收方提前收到较后部分
设提前收到 m+1 ~ N 的segment; 窗口大小为N
1. 接收方没有收到 左边界~m 的segment; 只发送左边界-1的ack
2. 发送方等待ack超时；重发 1~N 的segment
### 场景4: 窗口内segment 接收方只接收到前半部分(连续的)
设只收到 m 个segment, 窗口大小N
1. 接收方只发送第 m 个segment的ack; 窗口右移 m
2. 发送方收到第k个segment的ack; 窗口也右移k
3. 发送方 发送的 m-N个segment超时重发

# 4. 拥塞控制
https://blog.csdn.net/qq_33436509/article/details/81979024
https://www.cnblogs.com/alifpga/p/7675850.html (后半部分)

滑动窗口是控制接收以及同步数据范围的，通知发送端目前接收的数据范围，用于流量控制，接收端使用。拥塞窗口是控制发送速率的，避免发的过多，发送端使用。因为tcp是全双工，所以两边都有滑动窗口。
两个窗口的维护是独立的，滑动窗口主要由接收方反馈缓存情况来维护，拥塞窗口主要由发送方的拥塞控制算法检测出的网络拥塞程度来决定的。

# 2MSL 等待时间
https://www.cnblogs.com/zxpo/p/5234220.html
## 运行机制
1. 主动关闭者(FIN_WAIT_2) 发出最后一个ack 后进入TIME_WAIT
    + ack 网络中可能会被丢掉
2. 被动关闭者(LAST_ACK) 如果没有等到最后的ack；则定是重发 fin
3. 主动关闭者(TIME_WAIT) 如果在2MSL期间收到FIN; 则重发最后一个ack; 并再次等待2MSL
## 设计背景
1. 客户端关闭连接后再向服务端重新发起连接
2. 无法保证客户端是否复用了同一个本地端口号
3. 如果恰好使用了同一个端口号; 且上一次连接的数据报文存在于网络中
4. 那么上一次连接的数据报文重新进入本次连接; 导致数据的混淆
## Q: 2MSL 导致TIME_WAIT的socket在进程中长时间占据fd 无法释放


# close 和 shutdown 的区别
https://blog.csdn.net/justlinux2010/article/details/20913755
## 标准术语
1. close 是文件系统层的关闭
    + 单个进程close一个fd后不保证马上释放连接
    + 当fd在内核中的引用数为0才会释放连接
2. shutdown 直接对socket层的关闭 (不处理引用计数) (不能shutdown非socket的fd)
    + 调用后; 其他进程占有这个连接的fd; read返回EOF; write可能触发SIGPIPE(buffer满了)
    + 对于处于 TCP_CLOSE 状态的socket 会返回 ENOTCONN错误
3. shutdown 可以选择性 只关闭读 or 只关闭写 or 同时关闭读写
    + 但 close 必须同时关闭读写方向
4. shutdown只关闭socket不释放fd; 所以进程也要调用close释放fd
    + 但 close 释放fd 释放socket

## shutdown 实现
+ `int shutdown(int sockfd, int how);`
    1. how = SHUT_RD / SHUT_WR / SHUT_RDWR
```c++
sys_shutdown() {
    // 1. 根据fd 索引socket
    // 2. 根据socket 的 sk_common 结构体 的 协议方法 sk_prot 的 shutdown 释放
    // 3. udp 和 tcp 的 shutdown 方法都是 inet_shutdown
    sockfd_lookup_light(); 
    socket->sk->sk_prot->shutdown(sk, how);
}
inet_shutdown() {
    // 1. tcp 有 tcp_shutdown() 的额外回调
    // 2. udp  没有; 只有内存释放
}
tcp_shutdown() {
    // 1. 针对  TCP_ESTABLISHED、TCP_SYN_SENT、TCP_SYN_RECV TCP_CLOSE_WAIT
    //    调用 tcp_close_state() 状态变更
    tcp_close_state();
}
tcp_close_state() {
    // 1. 如果状态状态变更 则 tcp_send_fin()
    tcp_send_fin();
}
```

## close 实现
+ 调用链
```c++
sys_close()  // 根据 fd 类型调用释放方法
    > fd->file->filp_close();  // file 是 struct file
    > __fput()
    > fd->file->f_op->release(); // socket 的 release 函数是 sock_close
sock_close() // 根据 sk 类型调用释放方法
    > inet_release()    // tcp 和 udp
    > raw_release()     // raw
    > netlink_release() // netlink
    > unix_release()    // unix 套接字
inet_release() // 根据协议簇调用释放方法
    > tcp_close()       // struct proto tcp_prot;
    > udp_lib_close()   // struct proto udp_prot;
    > ping_close()      // struct proto ping_prot;
    > raw_close()       // struct proto raw_prot;
tcp_close();
```

分包
要发送的数据大于TCP缓冲剩余空间
待发送数据大于MSS（最大报文长度）

粘包:
要发送的数据小于TCP缓冲区，将多次写入缓冲区的数据一起发送
接收端的应用层没有及时读取缓冲区的数据
自定义通信协议的两种方式:
  定义数据包包头
  在数据包之间设置边界(即定义包头 包尾)