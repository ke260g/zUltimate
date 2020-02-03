// driver-layer > ... > sock_queue > ... > user-layer
// 驱动例子 drivers/net/ethernet/realtek

// 收包主要调用链(以udp为例)上部分, driver > sock_queue 
rtl8139_rx                { 驱动中断收包, backlog逻辑调用或直接调用netif_receive_skb }
eth_type_trans            { Link层 统一接口, 在驱动代码中调用, 剥掉link层 }
netif_receive_skb         驱动适配 统一的收包函数
__netif_receive_skb_core  驱动适配 收包实际处理函数 // TODO: 重点关注 特别是处理VLAN的部分
deliver_skb               link层到ip层
pt_prev->func             已注册的ip层回调 如 { ip_packet_type, arp_packet_type }
ip_rcv                    { 检查校验码, 检查header长度, NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING) }
ip_rcv_finish             { early_demux, ip_route_input_noref, ip_rcv_options, st_input(skb)  }
ip_route_input_noref      判定组播还是单播          
ip_route_input_slow       非组播包处理逻辑          // TODO: 重点关注 fib_XXX逻辑
ip_local_deliver          ip协议相关逻辑 组包 分包
ip_local_deliver_finish   { 剥掉sk_buff的网络层, 处理raw_socket逻辑, 调用传输层回调 }
udp_rcv                   { 直接调用__udp4_lib_rcv(IPPROTO_UDP) }
__udp4_lib_rcv            { 获取NF_HOOK塞进来的socket, udp组播逻辑, 根据src-port&dst-port查表获取socket }
udp_queue_rcv_skb         { backlog逻辑调用或直接调用__udp_queue_rcv_skb  }
__udp_queue_rcv_skb       { 几乎直接调用 sock_queue_rcv_skb }
sock_queue_rcv_skb        { 执行BPF过滤器 }
__sock_queue_rcv_skb      { __skb_queue_tail(sk->sk_receive_queue) }

// 收包主要调用链(以udp为例)下部分, user > sock_queue
SYSCALL_DEFINE5(recvmmsg) { 处理用户态超时逻辑, copy_from_user(timespec), }
__sys_recvmmsg            { 处理超时等待, 根据fd获取socket }
___sys_recvmsg            { }
sock_recvmsg_nosec        { 调用__sock_recvmsg }
sock_recvmsg              { 调用__sock_recvmsg }
__sock_recvmsg            { }
inet_recvmsg              { }
udp_recvmsg               { }
__skb_recv_datagram       { }
skb_copy_datagram_iovec   { }
memcpy_toiovec            { }
copy_to_user              { }
volatile_memcpy           { }

// 链路层 VLAN处理
__netif_receive_skb_core skb_vlan_untag vlan_do_receive

// 网络层 组播播调用链
ip_route_input_noref ip_route_input_mc ip_local_deliver // 组播 最后还是跟单播一样 ip_local_deliver
// 网络层 转发调用链 
ip_route_input_noref ip_route_input_slow ip_mkroute_input __mkroute_input ip_forward
// 网络层 raw_socket 调用链
ip_local_deliver_finish raw_local_deliver raw_v4_input raw_rcv
//////////////////////////////////////////////////

// 传输层 UDP收包组播调用链
__udp4_lib_rcv __udp4_lib_mcast_deliver
// 传输层 UDP收包csum调用链
skb_copy_and_csum_datagram_iovec skb_copy_and_csum_datagram_iovec skb_copy_and_csum_datagram 
// 传输层 TCP调用链
//////////////////////////////////////////////////


// 驱动层 调用栈
netif_receive_skb    netif_receive_skb(skb);
rtl8139_rx           rtl8139_rx(dev, tp, budget);
rtl8139_poll         netif_napi_add(dev, &tp->napi, rtl8139_poll, 64);
rtl8139_init_one     static struct pci_driver rtl8139_pci_driver = { .probe = rtl8139_init_one };
rtl8139_init_module  pci_register_driver(&rtl8139_pci_driver);
module_init(rtl8139_init_module);
////////////////////////////////////////

// __netif_receive_skb 调用栈1 先缓存 后处理
// 调用栈1::先缓存
sk_buff_head::input_pkt_queue      // 入列: 单个CPU的处理队列
enqueue_to_backlog
netif_receive_skb
// 调用栈2::后处理
__netif_receive_skb_core
__netif_receive_skb  __netif_receive_skb(skb);
process_backlog      sd->backlog.poll = process_backlog; // 每个CPU softnet_data即sd
net_dev_init         subsys_initcall(net_dev_init);

// __netif_receive_skb 调用栈2 直接处理
__netif_receive_skb_core
__netif_receive_skb         
netif_receive_skb_internal  return netif_receive_skb_internal(skb); // netif_receive_skb直接return
netif_receive_skb           
////////////////////////////////////////