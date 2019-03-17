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

## 过滤器
### 三种关键字
类型关键字 ho9t net port
方向关键字 src dst
协议关键字 fddi arp rarp ip tcp udp http ftp smtp
其他关键字 gateway broadcast
运算关键字 not and or less greater
### 例子
```sh
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
```