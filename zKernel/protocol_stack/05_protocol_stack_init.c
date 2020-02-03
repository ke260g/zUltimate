static int __init proto_init(void);      // /proc/net 建立相应protocols, 文件操作函数
static int __init net_dev_init(void);    // /proc/sys中建立网卡相关, 开启网卡中断, CPU收包逻辑, loopback
static int __init sock_init(void);       // initalize sk buff slab cache, ?指定是文件系统吗?
static int __init netpoll_init(void);
static int __init netlink_proto_init(void);
static __init int sysctl_core_init(void);
static __init int net_inuse_init(void);  // 
static int __init inet_init(void);       // 初始化 协议栈的主要函数

subsys_initcall(proto_init);
subsys_initcall(net_dev_init);
core_initcall(sock_init);
core_initcall(netpoll_init);
core_initcall(net_inuse_init);
core_initcall(netlink_proto_init);
fs_initcall(inet_init);
fs_initcall(sysctl_core_init);

static int __init inet_init(void) {
    // 初始化 struct proto tcp_prot, udp_prot, ping_prot, raw_prot;
    // 构成链表
    proto_register(&tcp_prot, 1);
    proto_register(&udp_prot, 1);
    proto_register(&ping_prot, 1);
    proto_register(&raw_prot, 1);
    
    // 注册 socket层的 inet 协议簇
    // 注入容器 static const struct net_proto_family *net_families[NPROTO];
    // 主要是 create 回调
    (void)sock_register(&inet_family_ops);

    // /sys/net/ipv4/route
    ip_static_sysctl_init();

    // 注册 网络层->传输层 的回调
    // 注入容器 inet_protos
    inet_add_protocol(&icmp_protocol, IPPROTO_ICMP);
    inet_add_protocol(&udp_protocol, IPPROTO_UDP);
    inet_add_protocol(&tcp_protocol, IPPROTO_TCP);
    inet_add_protocol(&igmp_protocol, IPPROTO_IGMP);

    // 初始化容器 inetsw
    // 本质: 注册 socket 的各个类型回调
    for (r = &inetsw[0]; r < &inetsw[SOCK_MAX]; ++r)
		INIT_LIST_HEAD(r);
    // 把 inetsw_array 注入容器inetsw中
	for (q = inetsw_array; q < &inetsw_array[INETSW_ARRAY_LEN]; ++q)
		inet_register_protosw(q);

    // 一系列协议模块的初始化
    arp_init(); // 调用 dev_add_pack(&arp_packet_type) link层注册arp协议回调
    ip_init();
    tcp_init();
    udp_init();
    ping_init();
    icmp_init();

    ipv4_proc_init(); // /proc/net/ipv4
	ipfrag_init();    // IP分片逻辑

    // link层注册ip协议回调
    // 注入容器 ptype_base[] 中
    dev_add_pack(&ip_packet_type);
}
// socket.c 中实现一系列 系统调用
// 

static int __init net_dev_init(void) {
    dev_proc_init();
    netdev_kobject_init();

    // 初始化 ptype_all ptype_base[] 容器
    INIT_LIST_HEAD(&ptype_all);
	for (i = 0; i < PTYPE_HASH_SIZE; i++)
		INIT_LIST_HEAD(&ptype_base[i]);
    
    register_pernet_subsys(&netdev_net_ops);

    /*
	 *	Initialise the packet receive queues.
	 */
	for_each_possible_cpu(i) {
		struct softnet_data *sd = &per_cpu(softnet_data, i); // 每个CPU

		skb_queue_head_init(&sd->input_pkt_queue);           // skb 输入队列
		skb_queue_head_init(&sd->process_queue);             // skb 处理队列
		INIT_LIST_HEAD(&sd->poll_list);                      // epoll 初始化
		sd->output_queue_tailp = &sd->output_queue;          // skb 输出队列
        // ...
	}

    // loopback 初始化
    register_pernet_device(&loopback_net_ops);

    register_pernet_device(&default_device_ops);

    // 收发中断
	open_softirq(NET_TX_SOFTIRQ, net_tx_action);
	open_softirq(NET_RX_SOFTIRQ, net_rx_action);

    hotcpu_notifier(dev_cpu_callback, 0);
    // ...
}