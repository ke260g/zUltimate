学习框架:
level 1: 协议本身 + unix编程接口 + 小工具 + 特殊场景解决方案(nginx libevent ss)
level 2: 内核 /proc/net

TCP IP 超简总结 https://www.jianshu.com/p/9f3e879a4c9c

unix编程接口
man 7 udp tcp
      raw icmp
      socket ip arp
      netlink ipfw
      sendmsg recvmsg
man 3 byteorder bswap endian (字端控制)
man netdb.h                  (名义上是数据库 实际上可获取服务端信息)

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