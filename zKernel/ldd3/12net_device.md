Network interfaces are the third standard class of Linux devices. (1. char; 2. block)

not in /dev/
not file_operations
Several hundred sockets can be multiplexed on the same physical interface.

块设备   只需要 响应 内核的请求
网络设备 还需要 响应 收包; 然后回传到协议栈
一系列 administrative tasks:
 setting addresses, modifying transmission parameters,
 maintaining  traffic  and  error  statistics

The  network  subsystem  of  the  Linux  kernel  is  designed  to  be  completely  protocol-independent.

drivers/net/loopback.c