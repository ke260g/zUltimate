获取桥的子接口 brctl show
获取vlan子接口 ip link show | grep vlan;  vlan$id@$sub_interface

ethtool -K eth0 rx-fcs on rx-all on 
控制网卡的属性

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

设置mtu 
ioctl(3, SIOCSIFMTU, {ifr_name="ge0_0_26", ifr_mtu=1600}) = 0

三层口需要关掉mac地址学习; 且从三层口上来的报文不会学习到二层mac表中

ovsdb-client transact '["OpenSwitch", {"op": 2, "table": "Interface", "where": [["name", "==", "ge0_0_8"]], "row": {"admin_state":false}}]'

ioctl 设置网卡; fd 都是随便的 socket() 即可; 然后 ifr_name 作为源只是设置的 net_device

/proc/net/dev 网卡列表一览
/sys/class/net/$interface 通过 access 判定网卡是否存在

https://0xax.gitbooks.io/linux-insides/content/Concepts/linux-cpu-1.html 另一个内核学习网站
https://blog.csdn.net/ustc_dylan/article/details/4049647                 RCU 解读网站1