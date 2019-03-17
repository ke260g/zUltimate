# Switch
+ 二层交换机
    + 只有端口 无MAC 用ASIC
    + 有环路问题
+ 三层交换机
    + 有n{IP/端口} + 1个mac 
    + cpu + asic/fpga
    + 有linux系统的
    + 无环路问题

### 问题
二层 三层交换机 路由器 三者有什么区别
    二层交换机 自己压根就没有mac 有端口+mac地址都表 二层交换机不解析ip协议

二层交换机 完全reset后 有机子插入到端口后 的学习过程
三层交换机 完全reset后 有机子插入到端口后 的学习过程


### 以太网协议
unicast   首bit为0 
multicast 首bit为1
broadcast ff-ff-ff-ff-ff-ff

交换机的纠错学习

### 二层交换机
+ 二层交换表 采用 定期清空/被动更新 的策略
##### PCa ARP获取 PCb mac 的流程
1. PCa send arp-request
d-mac broadcast
s-mac mac-a
d-ip  ip-b
s-ip  ip-a
2. SWx 交换机 recv
mac-a & port-a 更新到 二层交换表
看到 d-mac == broadcast, 即广播到各个端口
3. PCb recv
match d-ip
mac-a ip-a set to route-table
4. PCb send ARP-response
s-mac mac-b
d-mac mac-a
s-ip  ip-b
d-ip  ip-a
5. SWx 交换机 recv
mac-b & port-b 更新到 二层交换表
match mac-a & port-a
6. SWx 交换机 单播到 PC-a
##### SWx table 全部清空后
1. PCa send (d-mac mac-c)
2. SWx not match mac-c
3. SWx 所有port转发 packet
4. util PCc reponse
6. SWx update mac-c & port-c
7. PCa receive mac-c
##### PCb mac地址改了 PCa/SWx 均不知道
+ 即 SWx 转发packet后没有响应
1. 当PCb 收到 d-mac != mac-b 时
2. how to next ?
我猜
3. PCb 发送 ICMP 包
4. SWx更新 mac-b & port-b
5. PCa更新 mac-b & ip-b
##### 二层交换机环路问题

### 三层交换机
+ 硬件转发表 arp表 路由表
+ 任何包都会 update FDB
+ arp包才会 更新 arp表
+ 初始化时 只有PCa的路由表 和 switch的路由表
    + vlan-table 为空
    + arp-tables 为空
##### PCa(1.2) - [ (1.1) mac-s (2.1) ] - PCb(2.2)
+ 括号为一个ip 中括号为一个 switch
1. PCa send arp-request arp广播
    d-mac ff-ff-ff-ff-ff-ff & d-ip 1.1
    router-table 已知 (1.1) 
    arp-table    未知 mac-s
2. switch recv
    update arp-table,  ip-a <=> mac-a
    update vlan-table  (mac-a ip-a vlan-a)
3. switch send arp-response to PCa
4. PCa recv
    update arp-table, 1.1 <=> mac-s
5. PCa send tcp-request
    d-mac: mac-s & d-ip: ip-b
    已知 ip-b 未知 mac-b
6. switch recv
    update arp-table ip-a <=> mac-a
    update vlan-table (mac-a ip-a vlan-a)
    FDB try match ip-b (找到则转发给ip-b)
    FDB not match then router-table try match
    if find ip-b and match arp-table then send
    else find ip-b's subnet
7. switch send arp-request to ip-b's subnet
    d-mac: ff-... & d-ip: ip-b
    s-mac: mac-s  & s-ip: 192.168.2.1
8. PCb recv
    update arp-table (2.1 <=> mac-s)
9. PCb send arp-response
    d-mac: mac-s & d-ip: 2.1
10. switch recv
    update arp-tables (mac-b ip-b)
    update vlan-table (mac-b ip-b vlan-b)
11. switch send tcp-request (get from PCa) to PCb
    s-mac: mac-s s-ip: ip-a
12. finally all PCa <=> PCb via vlan-table