获取桥的子接口 brctl show
获取vlan子接口 ip link show | grep vlan;  vlan$id@$sub_interface


ethtool -K eth0 rx-fcs on rx-all on 
控制网卡的属性


+ ko 版本魔数 对不上
1. 获取目标机子的 /proc/version
2. 改源码 linux-4.14.76/include/config/kernel.release
3. 改源码 linux-4.14.76/include/generated/utsrelease.h
4. 编模块
5. grep 一下模块里面是不是真的有


dev_queue_xmit 是否需要释放内存??

+ dev_forward_skb 与 dev_queue_xmit 有什么区别?
    1. dev_forward_sbk = 使得 dev '收到报文'
    2. dev_queue_xmit  = 使得 dev '发出报文'

内核聚合口:
1. 成员口从 自造桥 中去掉
2. 聚合口加入到 网桥中
3. 成员口把收到的报文转发到聚合口, 如何实现??
4. 聚合口把发出的报文转发到成员口, 如何实现??
5. 聚合口的内核本质是什么, kernel_bond

内核 bonding
bond口列表      = /sys/class/net/bonding_masters            
bond口平衡模式  = /sys/class/net/$interface/bonding/mode []
bond口的配置    = /proc/net/bonding/$interface
bond口子接口    = /sys/class/net/$interface/bonding/slaves
    echo "+subinterface" > slaves
    echo "-subinterface" > slaves
bond口子接口    = /sys/class/net/$interface/bonding/active_slave
bond口子接口    = /sys/class/net/$interface/slave_$slave
bond口子接口    = /sys/class/net/$interface/lower_$slave

设置mtu 
ioctl(3, SIOCSIFMTU, {ifr_name="ge0_0_26", ifr_mtu=1600}) = 0



三层口需要关掉mac地址学习; 且从三层口上来的报文不会学习到二层mac表中

ovsdb-client transact '["OpenSwitch", {"op": 2, "table": "Interface", "where": [["name", "==", "ge0_0_8"]], "row": {"admin_state":false}}]'

mount -t debugfs nodev /sys/kernel/debug/

运行时内核函数地址
https://blog.csdn.net/gatieme/article/details/78310036


make -C $KENNEL_DIRECTORY M=$MODULE_DIRECTORY modules

ioctl 设置网卡; fd 都是随便的 socket() 即可; 然后 ifr_name 作为源只是设置的 net_device

