// syscall > inet > udp > ip > driver-compact > driver

// 发包调用链 系统调用
SYSCALL_DEFINE5(sendmsg) { 调用__sys_sendmsg }
__sys_sendmsg            { 根据fd找socket, 调用___sys_sendmsg发包 }
___sys_sendmsg           { 获取用户态msghdr, 分配足够多的iovec, 处理ctl控制参数, 切到内核态, 发包 }
sock_sendmsg             { __sock_sendmsg }
sock_sendmsg_nosec       { __sock_sendmsg_nosec }
__sock_sendmsg           { security_socket_sendmsg, __sock_sendmsg_nosec }
__sock_sendmsg_nosec     { sock->ops->sendmsg }
inet_sendmsg             { inet_autobind内核层必须bind, sk->sk_prot->sendmsg }

// 发包调用链 udp
udp_sendmsg              { } // TODO: 重点关注       
ip_route_output_flow
ip_make_skb              { } // TODO: 重点关注
udp_send_skb

// 发包调用链 网络层
ip_send_skb
ip_local_out
ip_local_out_sk
__ip_local_out
dst_output_sk
ip_output
ip_finish_output
ip_finish_output2

// 发包调用链 驱动适配
dst_neigh_output
dev_queue_xmit
__dev_queue_xmit
__dev_xmit_skb
dev_hard_start_xmit    { ops->ndo_start_xmit }
rtl8169_start_xmit

// ip_route_output_flow
ip_route_output_flow
__ip_route_output_key
__mkroute_output
rt_dst_alloc            { 赋值ip_output }

// ip_make_skb
ip_make_skb
__ip_append_data
ip_ufo_append_data
skb_append_datato_frags { 用户态数据复制到内核态 }