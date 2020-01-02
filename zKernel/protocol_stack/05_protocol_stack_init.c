static int __init proto_init(void);      // /proc/net 建立相应protocols
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
    proto_register(&tcp_prot, 1);
    proto_register(&udp_prot, 1);
    proto_register(&ping_prot, 1);
    proto_register(&raw_prot, 1);
    
    // 注册 socket层的 inet 协议簇
    // 注入容器 static const struct net_proto_family *net_families[NPROTO];
    (void)sock_register(&inet_family_ops);

    // 注册 网络层->传输层 的回调,
    // 注入容器 inet_protos
    inet_add_protocol(&icmp_protocol, IPPROTO_ICMP);
    inet_add_protocol(&udp_protocol, IPPROTO_UDP);
    inet_add_protocol(&tcp_protocol, IPPROTO_TCP);
    inet_add_protocol(&igmp_protocol, IPPROTO_IGMP);

    // 初始化容器 inetsw
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
	ipfrag_init();    // 分片逻辑

    dev_add_pack(&ip_packet_type); // link层注册ip协议回调
}
// socket.c 中实现一系列 系统调用
// 