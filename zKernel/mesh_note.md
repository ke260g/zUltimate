获取桥的子接口 brctl show
获取vlan子接口 ip link show | grep vlan;  vlan$id@$sub_interface


ethtool -K eth0 rx-fcs on rx-all on 
控制网卡的属性


ko 版本魔数 对不上
1. 获取目标机子的 /proc/version
2. 改源码 linux-4.14.76/include/config/kernel.release
3. 改源码 linux-4.14.76/include/generated/utsrelease.h
4. 编模块
5. grep 一下模块里面是不是真的有


dev_queue_xmit 是否需要释放内存??
dev_forward_skb 与 dev_queue_xmit 有什么区别?
dev_forward_sbk = 使得 dev '收到报文'
dev_queue_xmit  = 使得 dev '发出报文'
