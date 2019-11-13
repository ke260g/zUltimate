学习框架:
level 1: 协议本身 + unix编程接口 + 小工具 + 特殊场景解决方案(nginx libevent ss)
level 2: 内核 /proc/net

TCP IP 超简总结 https://www.jianshu.com/p/9f3e879a4c9c

unix编程接口
man 7 udp tcp icmp
      socket ip arp
      netlink ipfw
      sendmsg recvmsg
man 3 byteorder bswap endian (字端控制)
man netdb.h                  (名义上是数据库 实际上可获取服务端信息)
snmp

网际协议
apt install quagga
ripngd(8), bgpd(8), ripd(8), ospfd(8), ospf6d(8), isisd(8), zebra(8), vtysh(1)

应用层 DNS HTTP FTP SSL DHCP
传输层 TCP UDP ICMP IGMP ICMPv6
网络层 (ip(0800) arp rarp) (rip bgp ospf) (ipv6)
链路层 CSMA 802 PPP SLIP CSLIP MTU

IPv4 address mask broadcast multicast 分包和组包
TCP  三次握手 四次握手 seq与ack 超时重传 segment window window-retrans
IPv6
segment/packet/frame 段/包/帧 传输层/网络层/链路层

### 路由表流程示例
<pre>
如果 找到dest-ip 完全相同的项,
	arp流程学习 目标主机 MAC
如果 同子网 255.255.255.0
	则发给 子网路由器, 如果有
	arp流程学习 子网路由器 MAC
如果 同网号 255.255.0.0
	则发给 网号路由器, 如果有
	arp流程学习 网号路由器 MAC
发给默认路由器
	这个必须有
	arp流程学习 默认路由器 MAC
</pre>

### resource
+ 简明的总结 https://www.sans.org/security-resources/tcpip.pdf


### Self Conclusion
一个通讯流程的抽象至少以下部分组成:
角色描述, 通讯时序, 通讯内容, 数据流向
#### 角色描述
至少包括角色集合, 升华版可以是关系描述
例子:
TCP(C/S)
SNMP(MasterAgent, Manager, Agent)
802.1x(supplicant, Authenticator, Authentication server)
到了更复杂的场景,  就需要考虑拓扑关系
#### 通讯时序-状态机
可以采用至少两种状态机模型
+ 每个角色的状态机, "变换函数"是可以"事件" 或[收/发 通讯内容](#通讯内容)
+ 全局上下文抽象状态机, 每个状态下是 不同角色的状态tuple 
#### 通讯内容
协议头描述
数据内容的组成结构
TCP/IP 的话要指明端口
#### 数据流向
物理流向  抽象流向


接入层确实会多开一些业务，比如防环，认证。
然后核心层的 硬件转发性能要好一点，MAC地址表大一点，甚至有的交换机灾备的需求。