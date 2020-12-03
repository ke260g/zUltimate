[TOC]
分析 /proc/net 目录下的文件  (/proc/sys/net 在其他篇章分析)

+ `/proc/net/arp`       arp 表; 这里看不到生命周期
+ `/proc/net/dev`       网卡表; 基本是 ifconfig 看到; 内核的 init_net 表
+ `/proc/net/dev_mcast` 组包表; 组播地址<->网卡 映射表
+ `/proc/net/ptype`     已注册回调的二层报文协议, 内核接口 `dev_add_pack(struct packet_ptype)`
+ `/proc/net/route`     对应 `>_ route -n`
    + 包括: 目的IP 网关IP 掩码 标记 跃点(Metric) 引用计数 使用计数 网卡 

# fib
+ `/proc/net/fib_trie`     三层表;
+ `/proc/net/fib_triestat` 表项统计;

# iptables 
+ `/proc/net/ip_tables_names`   iptables -t 选项的参数, 即 table 列表
+ `/proc/net/ip_tables_matches` iptables -p 选项的参数, 即 可以配置的协议
+ `/proc/net/ip_tables_targets` iptables -j 选项的参数, 即 可以配置的目标
    + DNAT / SNAT / CONNMARK / MARK / REJECT / MASQUERADE / ECN / ERROR / 
      TCPMSS / REDIRECT / NETMAP / TTL

# socket()
+ `/proc/net/netlink`      用户态所有 `socket(AF_NETLINK, socket_type, netlink_family)` 列表
    + 内有 inode; 与 `ls -l /proc/[pid]/fd/` 中的 inode 对应
+ `/proc/net/raw`          用户态所有 `socket(AF_INET, SOCK_RAW, protocol)` 列表
    + 内有 inode; 与 `ls -l /proc/[pid]/fd/` 中的 inode 对应
+ `/proc/net/packet`       用户态所有 `socket(AF_PACKET, socket_type, protocol)` 列表
    + 内有 inode; 与 `ls -l /proc/[pid]/fd/` 中的 inode 对应
+ `/proc/net/tcp`          用户态所有 `socket(AF_INET, SOCK_STREAM, 0)` 列表
    + 内有 inode; 与 `ls -l /proc/[pid]/fd`  中的 inode 对应
    + 与 `netstat -tna` 对应
+ `/proc/net/udp`          用户态所有 `socket(AF_INET, SOCK_DGRAM, 0)` 列表
    + 内有 inode; 与 `ls -l /proc/[pid]/fd`  中的 inode 对应
    + 与 `netstat -una` 对应
+ `/proc/net/unix`         用户态所有 `socket(AF_UNIX, type, 0)` 列表
    + 内有 inode; 与 `ls -l /proc/[pid]/fd`  中的 inode 对应
    + 与 `netstat -xna` 对应
+ `/proc/net/protocols`    用户态所有 `socket()` 类型的统计
    1. 统计的类型: packet / unix /  ping / raw / udp / tcp
    2. 统计的项目: 大小 / 个数
+ `/proc/net/sockstat`     用户态所有在用 `socket()` 的个数统计
    ```txt
    >_ cat /proc/net/sockstat
    sockets: used 300
    TCP: inuse 72 orphan 0 tw 5 alloc 77 mem 394
    UDP: inuse 29 mem 1
    UDPLITE: inuse 0
    RAW: inuse 6
    FRAG: inuse 0 memory 18446744073709547776
    ```

# 协议报文统计
+ `/proc/net/snmp`         Ip, Icmp, IcmpMsg, Tcp, Udp, UdpLite 的基本统计信息
    + (实际上一般用 `>_ netstat -s`)
    + 如果 tcp::Rto算法 tcp::最大连接 tcp::当前打开的连接数
+ `/proc/net/netstat`      TcpExt 统计 和 IpExt 统计
    + (实际上一般用 `>_ netstat -s`)
    + 如 TW socket 的个数; TCPTimeouts 的次数

# vlan
+ `/proc/net/vlan/config` 记录 vlan 及其子接口的结构
    + 形同 `brctl show`; 但 vconfig 没有 show 这样的参数
+ 具体的 vlan
    ```js
    >_ cat /proc/net/vlan/vlan1
    vlan1  VID: 1    REORDER_HDR: 1  dev->priv_flags: 1001
        total frames received            0
        total bytes received            0
    Broadcast/Multicast Rcvd            0

    total frames transmitted         2029
    total bytes transmitted        87638
    Device: br_vnode_1
    INGRESS priority mappings: 0:0  1:0  2:0  3:0  4:0  5:0  6:0 7:0
    EGRESS priority mappings: 0:7
    ```