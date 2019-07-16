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