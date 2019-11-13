[TOC]
# netlink
man 3 libnetlink
man 3 netlink
man 7 netlink
man 3 rtnetlink
man 7 rtnetlink

+ Netlink  is  used to transfer information between the kernel and user-space processes.
+ It is often better to use netlink via libnetlink than via the low-level kernel interface.

# low-level kernel interface
netlink_socket = socket(AF_NETLINK, socket_type, netlink_family);
Netlink is a datagram-oriented service.
Both SOCK_RAW and SOCK_DGRAM are valid values for socket_type.

形式上, 形同socket_fd
功能上, 支持 读/写/订阅 相关网络事件
读写草在: 通过 
订阅操作: 通过 sockaddr_nl 对象的 nl_groups = RTMGRP_LINK/RTMGRP_IPV4_ROUTE/RTMGRP_IPV4_IFADDR 控制订阅事件集

netlink programming demo
https://www.jianshu.com/p/073bcd9c3b08
libnl programming demo
https://www.jianshu.com/p/e20da51e7263