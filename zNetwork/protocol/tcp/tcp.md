
### TCP 可靠连接
+ 三次握手
+ 四次握手
+ 自适应包大小 Nagle
+ 超时重传
+ 滑动窗口
+ 心跳包

### TCP加速
+ Nagle算法 等待足够多数据一起发
    + TCP_NODELAY 关掉这个算法
+ 携带ACK 等一会把 下一帧要发送的数据连着上一个ack一起发
+ 滑动窗口
+ 数据拥塞
+ 超复杂定时器
    超时定时器 坚持定时器 2MSL定时器

### 三次握手
client: send SYN seq=J
(client => ) (server => SYN_RCVD)
server: send SYN ACK seq=K ack=J+1
client: send ACK ack=K+1
(client => ESTABLISHED) (server => ESTABLISHED)

### 四次握手
client: send FIN=M
(client => FIN_WAIT_1) (server => CLOSE_WAIT)
server: send ack=M+1
(client => FIN_WAIT_2)
server: send FIN=N
(server => LAST_ACK)
client: send FIN=N+1
(client => TIME_WAIT) (server => CLOSED)

### 超时重传 ( seq + ack )
+ 三种情况 (正常 发送方的包未到达接收方 接收方的ack未到达发送方)
+ 发送方 针对长时间没收到ack的包 重发
+ 接收方 针对重复收到的已经发过ack的seq包 重新发ack
+ 发送方的**等待时长**是动态计算的
  + 首次等待 两倍 往返时长
  + 然后等待 四倍 往返时长
  + 足够多次重发后 直接断开连接
+ 每一对seq + ack 称为一个segment
+ segment大小在三次握手时 确认

### 滑动窗口
+ 显然一对一的 seq + ack 太慢了
+ 定义窗口 为N个segment 的大小
+ 一次性发送N个segment 等待N个ACK
+ 丢包本质有两种: 发送段包体丢了 接收端ack丢了
+ 发送端
  + 同一窗口中的segment 即使上一段没有收到ACK 也继续发
  + 窗口中**第**m个segment的ack收到后 即可把窗口 往后移m个segment
  + 当**第**m个segment的ack收到后  即可无视 0 ~ m-1 个segment的ack是否收到 即使没收到也无需重发
  + 避免因ack丢失 导致的冗余重发
  + 当**第**n个segment的ack重复收到 表明该segment的下一段 丢失了
+ 接受端 (移动窗口 + 发送首个非连续处segment的ack)
  + 同一窗口中的segment 当第i个segment没有收到 但收到了其后i+1, i+2的segment
  + 这时 接受端 每次收到i + j的segment时, 发第i个ack
  + 直到收到 i个segment后, 且窗口中segment完全连续时 发送最末端ack
  + 对于收到重复的seq包 不作特别处理  沿用上面的策略

### 状态 MSS MSL
+ TCP MSS = MTU - IP - TCP 一般是1460
+ CLI 状态
CLOSED->SYN_SENT->ESTABLISHED->FIN_WAIT_1->FIN_WAIT_2->TIME_WAIT->CLOSED
+ 服务器状态
CLOSED->LISTEN->SYN_RECV->ESTABLISHED->CLOSE_WAIT->LAST_ACK->CLOSED
+ 2MSL等待状态
2MSL等待状态
在TIME_WAIT2发送了最后一个ACK数据报以后，要进入TIME_WAIT状态，这个状态是防止最后一次握手的数据报没有传送到对方那里而准备的（注意这不是四次握手，这是第四次握手的保险状态）。这个状态在很大程度上保证了双方都可以正常结束，但是，问题也来了。
由于插口的2MSL状态（插口是IP和端口对的意思，socket），使得应用程序在2MSL时间内是无法再次使用同一个插口的，对于客户程序还好一些，但是对于服务程序，例如httpd，它总是要使用同一个端口来进行服务，而在2MSL时间内，启动httpd就会出现错误（插口被使用）。为了避免这个错误，服务器给出了一个平静时间的概念，这是说在2MSL时间内，虽然可以重新启动服务器，但是这个服务器还是要平静的等待2MSL时间的过去才能进行下一次连接


分包
要发送的数据大于TCP缓冲剩余空间
待发送数据大于MSS（最大报文长度）

粘包:
要发送的数据小于TCP缓冲区，将多次写入缓冲区的数据一起发送
接收端的应用层没有及时读取缓冲区的数据
自定义通信协议的两种方式:
  定义数据包包头
  在数据包之间设置边界(即定义包头 包尾)