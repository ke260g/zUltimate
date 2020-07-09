ldd3 at first

## 1. protocol-stack:
 level 1: preview trace udp
 level 2: Understanding Linux Network Internals ( vlan bridge )
 level 3: zero copy; fib_lookup
 + ip 的拆包组包
 + netfilter 研究方向:
    1. 在内核中搜 struct nf_hook_os 的引用
    2. 在内核中搜 NF_HOOK 的引用了解各个钩子节点挂了那些钩子
    3. 在内核中搜 NF_CT_STAT_INC 了解具体的统计计数 用于排查包流向

## 2. memory:
 slabinfo: trace the slabinfo entry in kernel, create by proc_create()
 kmemleak: have a look on kmemleak dump file
 https://linux-kernel-labs.github.io/refs/heads/master memmory section

## 3. more = ( lkd3 ulk )


+ 学习完一轮 ldd3 后拆分为若干个专题; 假装自己没看过ldd3

中断todolist


# 中断
定时器的实现
workqueue 再次深入了解

# 网络
网络组播
协议栈发包
网卡驱动的 netpoll
netlink
netfilter
bridge 转发
vlan 转发
nftable
rtnl_link_register
notifier_block; register_netdevice_notifier

# 调度
进程调度

# 内存
内存屏障
原子变量的底层实现
RCU
自旋锁的实现
中断的实现

# 驱动
misc
