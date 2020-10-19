小林 coding 中有很多至关重要的 tcp 理论, 看一下ta的博客
https://www.cnblogs.com/xiaolincoding/p/12995358.html ()

syn-cookies
https://blog.csdn.net/onetwothreef/article/details/8894690?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromBaidu-1.nonecase
https://blog.csdn.net/weixin_42104231/article/details/83655742?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromBaidu-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromBaidu-1.nonecase

timestamps
https://www.cnblogs.com/yfceshi/p/6807414.html
https://segmentfault.com/a/1190000000736974

tcp笔记框架重构
1. tcp.question.md
2. tcp.sysctl/  调参
3. tcp.window/
4. tcp.state_machine/

// 一些参数的深入理解
   tcp.parameter/
5. tcp_syncookies.md
6. tcp_timestamps.md
高并发调优
https://segmentfault.com/a/1190000000473365?utm_source=sf-related
https://segmentfault.com/a/1190000023919222?utm_source=sf-related


滑动窗口 一种可以让 「发送方」根据「接收方」的实际接收能力控制发送数据量 的机制
接收窗口并不是恒定不变的 
接收方会把当前可接收的大小放在 TCP 报文头部中的窗口字段
从而通知发送方



这个窗口大小最大值，在当今高速网络下，很明显是不够用的。
所以后续有了扩充窗口的方法：在 TCP 选项字段定义了窗口扩大因子，
用于扩大 TCP 通告窗口，其值大小是 2^14，
这样就使 TCP 的窗口大小从 16 位扩大为 30 位（2^16 * 2^ 14 = 2^30），
所以此时窗口的最大值可以达到 1GB。
tcp-option-type 3 指示最大窗口扩展后的值 
/proc/sys/net/ipv4/tcp_window_scaling 开启

主动建立连接的一方在 SYN 报文中发送这个选项
被动建立连接的一方只有在收到带窗口扩大选项的 SYN 报文之后才能发送这个选项

所以 TCP 的传输速度 实际上，受制于发送窗口与接收窗口，以及网络设备传输能力
缓冲区与网络传输能力匹配，那么缓冲区的利用率就达到了最大化

网络设备传输能力   带宽是单位时间内的流量，表达是「速度」，比如常见的带宽 100 MB/s；
发送窗口接收窗口   缓冲区单位是字节，当网络速度乘以时间才能得到字节数

q: 发送方的窗口等价于接收方的窗口吗？
如果不考虑拥塞控制，发送方的窗口大小「约等于」接收方的窗口大小，
因为窗口通知报文在网络传输是存在时延的，所以是约等于的关系。
窗口字段只有 2 个字节，因此它最多能表达 65535 字节大小的窗口，也就是 64KB 大小。

q: 发送双方系统性能足够高; 能否无上限地提升 窗口 大小?
    不可能 网络的传输能力是有限的
    发送超过网络处理能力的报文时
    由器会直接丢弃这些报文

q: 如何确定最大传输速度 ?
  带宽时延积BDP = RTT(时延) * 带宽(网络设备决定)
  指示一个连接在网络中游离的报文总数据量
  如果超过这个数据量; 就会丢包
  而 发送缓冲区 决定了 发送窗口上限;
  且 发送窗口上限 决定了"已发送未确认"的报文上限 (游离的报文)
  如果 发送缓冲区 大于 带宽时延积; 网络设备过载; 丢包重传严重
  如果 发送缓冲区 小于 带宽时延积; 不能很好地发挥网络的性能
