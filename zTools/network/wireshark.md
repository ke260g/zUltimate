

## 过滤
逻辑运算符 `! == || && ()` 不要用 `!=`
逻辑关键字 and not or
Analyze -> Display Filters 有很多实例
### 过滤协议, 直接小写协议名
udp tcp 
arp icmp igmp
dns http https ftp smtp

### TCP
tcp.flags.xxx == 1
tcp.flags.xxx == 0
常用的flags: tcp.flags.syn tcp.flags.ack
过滤端口: tcp.port == 80

### 网络层过滤
ip.addr == 192.168.1.1
ip.src_host == 

### 链路层过滤
过滤广播包 eth.addr == ff:ff:ff:ff:ff:ff 
过滤IP包   eth.type == 0x0800
过滤地址   eth.addr == 00:00:xx:

### 具体协议的具体字段过滤
icmp.type == 8
!(http.accept_encoding == utf-8)

## Statistics > I/O Graph 网络包统计
+ Y Axis 可用函数: MIN AVG MAX SUM  COUNT LOAD
+ 统计图 如何 反映真实网络情况?
### 函数说明
+ COUNT 单位时间内 符合条件的包 发生次数
+ SUM   单位时间内 符合条件的量 总量
	+ 如tcp.len (tcp包的总吞吐量)
	+ 如tcp.seq
### 实际上 有哪些情况会出现 (其实本质只有延时, 延时太多就会丢失, 拥塞是延时的重要原因)
+ request  网络路径中延时大 才能到
+ response 处理时间太长 (cpu/io/mem)
+ response 网络路径中延时大,
+ request/response 网络路径中 包丢失(理论上 延时足够大就是丢失)
+ request/response 网络路径中 拥塞, 通常以窗口大小反映, 反映整个网络不堪重负
### 查看来回延时
+ Y Field := frame.time_delta
+ Y Axis  := [ MIN AVG MAX ]
### 查看重传情况1
+ Y Field := tcp.analysis.retransmission
+ Y Axis  := COUNT
### 查看重传情况2
+ Y Field := tcp.seq
+ Y Axis  := SUM
+ 网络正常时, 稳定增加, 然后缓慢降低, 再稳定增加, 然后循环
+ 经常重传时, seq 呈现高低起伏, 且偶尔会出现异常激增点
### 实用的tcp统计 Y Field
tcp.analysis.lost_segment：表明已经在抓包中看到不连续的序列号。报文丢失会造成重复的ACK，这会导致重传
tcp.analysis.duplicate_ack：显示被确认过不止一次的报文。大量的重复ACK是TCP端点之间高延时的迹象
tcp.analysis.retransmission：显示抓包中的所有重传。如果重传次数不多的话还是正常的，过多重传可能有问题。这通常意味着应用性能缓慢和/或用户报文丢失
tcp.analysis.window_update： 如果看到窗口大小下降为零，这意味着发送方已经退出了，并等待接收方确认所有已传送数据。这可能表明接收端已经不堪重负了
tcp.analysis.ack_rtt：衡量抓取的TCP报文与相应的ACK。如果这一时间间隔比较长那可能表示某种类型的网络延时(报文丢失，拥塞，等等)
tcp.analysis.bytes_in_flight： 未确认字节数不能超过你的TCP窗口大小（定义于最初3此TCP握手）, 为了最大化吞吐量你想要获得尽可能接近TCP窗口大小。如果看到连续低于TCP窗口大小，可能意味着报文丢失或路径上其他影响吞吐量的问题。