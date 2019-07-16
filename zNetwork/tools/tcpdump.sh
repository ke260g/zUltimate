#!/bin/bash
<< EOF
-D 显示有多少张可用网卡
-n 以数字显示ip及端口
-i eth0 指定网卡
-i any  所有网卡
-c $count 包数量
–w filename.log         输出文件
–r google.cap <过滤器>  输入文件 读取过滤器部分
-A 封包的內容以 ASCII 顯示，通常用來捉取 WWW 的網頁封包資料
-x 十六進位 (hex) 以及 ASCII 的封包內容，對於監聽封包內容很有用
-e 显示链路层的头
-d, -dd, -ddd  可阅读的格式, C语言的格式, 十进制数字的格式

过滤器的关键字类型:
类型关键字 ho9t net port
方向关键字 src dst
协议关键字 fddi arp rarp ip tcp udp http ftp smtp
其他关键字 gateway broadcast
运算关键字 not and or less greater
EOF

# 例子
# 210.27.48.1 的主机收到的和发出的所有的数据包
tcpdump host 210.27.48.1

# 210.27.48.1 和 { 210.27.48.2 | 210.27.48.3 }的通信
# 即 210.27.48.1 和 210.27.48.2 的通信
# 即 210.27.48.1 和 210.27.48.3 的通信
tcpdump host 210.27.48.1 and \(210.27.48.2 or 210.27.48.3 \)

# 210.27.48.1 和 除了210.27.48.2 的通信ip包
tcpdump ip host 210.27.48.1 and not 210.27.48.2

# 210.27.48.1接收或发出的telnet包
tcpdump tcp port 23 host 210.27.48.1

# 123 为ntp的服务端口
tcpdump udp port 123

# 监听 from 210.27.48.1 的包
tcpdump -i eth0 src host hostname
# 监听   to 210.27.48.1 的包
tcpdump -i eth0 dst host hostname

# 指定网关的包
tcpdump -i eth0 gateway Gatewayname

# tcpflag
tcpdump  "src host 200.200.95.50 and dst host 200.200.95.52 and tcp and tcp[tcpflags] & (tcp-syn|tcp-rst) != 0"

# '-A' convert to ascii
tcpdump -A -s0 port 80

# '-w' write to file
tcpdump -i eth0 -s0 -w test.pcap

# specify port via text
tcpdump -nn -v port ftp or ftp-data

# ipv6
tcpdump -nn ip6 proto 6

#####################################################################
# http scenario
# '-l' line mode, useful for many special scenario
sudo tcpdump -nn -A -s1500 -l | grep "User-Agent:"

<< EOF
((tcp[12:1] & 0xf0) >> 2)        := header's the number of bytes
tcp[((tcp[12:1] & 0xf0) >> 2):4] := 4 bytes after header
EOF
# match 'GET' request
tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x47455420'
# match 'POST request
tcpdump -s 0 -A -vv 'tcp[((tcp[12:1] & 0xf0) >> 2):4] = 0x504f5354'

# extract POST/GET/Host:
tcpdump -s0 -v -n -l | egrep -i "POST /|GET /|Host:"
# Extract HTTP Passwords in POST Requests
tcpdump -s0 -A -n -l | egrep -i "POST /|pwd=|passwd=|password=|Host:"
# Cookie
tcpdump -s0 -A -nn -l | egrep -i 'Set-Cookie|Host:|Cookie:'
#####################################################################

#####################################################################
# icmp scenario

# not standard icmp packet
tcpdump 'icmp[icmptype] != icmp-echo and icmp[icmptype] != icmp-echoreply'
#####################################################################