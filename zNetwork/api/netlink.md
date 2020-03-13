[TOC]
# netlink
+ Netlink  is  used to transfer information between the kernel and user-space processes.
+ It is often better to use netlink via libnetlink than via the low-level kernel interface.
Notes: iproute2 using netlink, net-utils using /proc/net

## user-space
man 3 libnetlink
man 3 netlink
man 7 netlink
man 3 rtnetlink
man 7 rtnetlink
man 3 cmsg
### function interface
netlink_socket = socket(AF_NETLINK, socket_type, netlink_family);
Netlink is a datagram-oriented service.
Both SOCK_RAW and SOCK_DGRAM are valid values for socket_type.

+ 形式上, 形同socket_fd
+ 功能上, 支持 读/写/订阅 相关网络事件
+ 三层通讯分流: socket_family + nl_groups + nl_type 
+ 读写操作: 通过 nl_groups = 0  struct nlmsghdr { nlmsg_type } 控制
  + RTM_NEWLINK, RTM_DELLINK, RTM_GETLINK(接口interface))
  + RTM_NEWADDR, RTM_DELADDR, RTM_GETADDR(IP地址)
  + RTM_NEWROUTE, RTM_DELROUTE, RTM_GETROUTE(路由表)
  + RTM_NEWNEIGH, RTM_DELNEIGH, RTM_GETNEIGH(ARP表)
  + RTM_NEWRULE, RTM_DELRULE, RTM_GETRULE
  + RTM_NEWQDISC, RTM_DELQDISC, RTM_GETQDISC
  +  RTM_NEWTCLASS, RTM_DELTCLASS, RTM_GETTCLASS
  + RTM_NEWTFILTER, RTM_DELTFILTER, RTM_GETTFILTER
+ 订阅操作: bind sockaddr_nl { nl_groups = RTMGRP_LINK/RTMGRP_IPV4_ROUTE/RTMGRP_IPV4_IFADDR } 控制订阅事件集
可靠性: 使用NLM_F_ACK, 但本质可靠性无法保证, 因为进程buf已满, 内核没有扩容。不过进程recv会返回ENOBUFS

### basic usage of NETLINK_ROUTE
Receives routing and link updates.
Modify the routing tables (both IPv4 and IPv6), IP addresses, link parameters, neighbor setups, queueing disciplines, traffic classes and packet classifiers.

GET Request
NEW Request

netlink programming demo
https://www.jianshu.com/p/073bcd9c3b08
libnl programming demo
https://www.jianshu.com/p/e20da51e7263


## kernel-space
1. 内核通过 socket_family, nl_groups实现netlink本身的路由
2. 通过 nlmsg_pid 寻址, 发给用户进程, 但是如果nlmsg_pid = 0, 即内核本身, 会有死循环, 必须处理 pid = 0的情况
```c
#include <linux/netlink.h> // 这是内核层的
/**
 * @param module THIS_MODULE by default
 * @param net init_net by default
 * @param unit socket_family
 * @param groups nl_group
 */
struct sock *netlink_kernel_create(struct net *net, int unit, unsigned int groups, void (*input)(struct sk_buff *skb), struct mutex *cb_mutex,struct module *module);

int netlink_unicast(struct sock *sk, struct sk_buff *skb, u32 pid, int nonblock);
void netlink_broadcast(struct sock *sk, struct sk_buff *skb, u32 pid, u32 group, int allocation);

void netlink_kernel_release(struct sock *sk);
```
对于内核, 也是返回一个fd, 但内核层只有读回调, 和unicast,broadcast接口
### demo communicate betweeen kernel and user
https://www.cnblogs.com/zhiliang9408/p/11370838.html
本质:
1. 用户层 socket_family = NETLINK_GENERIC/NETLINK_USER
2. nl_groups = 0
3. 自定义 nl_type


如果使用 netlink 作为同步内核和用户 的媒介;
设计上, 容易出现的性能问题是 内存双倍的问题.
如果是这种问题, 建议使用内核和用户 的共享内存