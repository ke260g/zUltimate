// --------------------------------------------------
// begin: socket层 >> link层:  famliy > socket-type > ip-protocol
// --------------------------------------------------
static const struct net_proto_family inet_family_ops;    // socket(AF_INET, , ); tcp 和 udp
static const struct net_proto_family inet6_family_ops;
static const struct net_proto_family unix_family_ops;    // socket(AF_UNIX, , );
static const struct net_proto_family netlink_family_ops; // socket(AF_NETLINK, , );
static const struct net_proto_family packet_family_ops;  // socket(AF_PACKET, , ); 二层报文监听
static const struct net_proto_family *net_families[NPROTO]; // 容器, xxx_family_ops->family 作为id

// send / recv / bind / connect / listen / shtudown 等 系统调用的回调
// 每个 net_proto_family 绑定一个or多个 proto_ops
// 在 net_proto_family 的 create 方法中赋值
const struct proto_ops inet_stream_ops;
const struct proto_ops inet_dgram_ops;
static const struct proto_ops inet_sockraw_ops;
static const struct proto_ops netlink_ops;

struct proto tcp_prot;
struct proto udp_prot;
struct proto raw_prot;
struct proto ping_prot;
static struct inet_protosw inetsw_array[] = {
    { SOCK_STREAM, IPPROTO_TCP,  &tcp_prot,  &inet_stream_ops,  },
    { SOCK_DGRAM,  IPPROTO_UDP,  &udp_prot,  &inet_dgram_ops,   },
    { SOCK_DGRAM,  IPPROTO_ICMP, &ping_prot, &inet_sockraw_ops, },
    { SOCK_RAW,    IPPROTO_IP,   &raw_prot,  &inet_sockraw_ops, },
};

// inet_init 遍历inetsw_array的每个成员, 调用inet_register_protosw()
// inet_register_protosw() 把 inet_protosw 对象入列 inetsw[]
//
// inet_sw[] inet_protosw->type, 即socket类型, 作为数组索引, 
// 每个数组成员中都有list, list成员对应一种层协议(ping/raw/udp/tcp)
static struct list_head inetsw[SOCK_MAX];
// --------------------------------------------------
// end: socket层 >> link层:  famliy > socket-type > ip-protocol
// --------------------------------------------------

// --------------------------------------------------
// begin: link层 > socket层
// --------------------------------------------------
static const struct net_protocol udp_protocol;
static const struct net_protocol tcp_protocol;
static const struct net_protocol icmp_protocol;
static const struct net_protocol igmp_protocol;
static const struct net_protocol sctp_protocol;
// 网络层 协议回调容器
extern const struct net_protocol *inet_protos[MAX_INET_PROTOS];
// 协议注册到inet_protos容器中, 2nd-argument is IPPROTO_XXX
int inet_add_protocol(const struct net_protocol *prot, unsigned char num);

static struct packet_type pppoes_ptype;
static struct packet_type ip_packet_type;
static struct packet_type arp_packet_type;
static struct packet_type rarp_packet_type;
static struct packet_type bootp_packet_type;
static struct packet_type ipv6_packet_type;
// Link层 协议回调容器, 结构上是 hash_list, PTYPE_HASH_MASK 是16
// 这么设计的原因是 ether_type 值贼大且发散, 总数只有10多个
// 例如 ip = 0x8000, 802.1Q = 0x8100
static struct list_head ptype_base[PTYPE_HASH_SIZE];
// 协议注册到容器中, packet_type->type & mask 作为索引, 然后加到链表头
void dev_add_pack(struct packet_type *pt);
// Link层(每个网络设备) 收到包后, 分发到 Link层协议的回调
// list_for_each_entry_rcu(ptype,
// 		&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list) {
// 	if (ptype->type == type && other_condition) {
//         ptype->func((skb);
//     }
// }
static int __netif_receive_skb_core(struct sk_buff *skb, bool pfmemalloc);
// --------------------------------------------------
// end: link层 > socket层
// --------------------------------------------------

// --------------------------------------------------
// begin: link层
// --------------------------------------------------
const struct header_ops eth_header_ops = {
	.create		= eth_header,
	.parse		= eth_header_parse,
	.rebuild	= eth_rebuild_header,
	.cache		= eth_header_cache,
	.cache_update	= eth_header_cache_update,
};

void ether_setup(struct net_device *dev)
{
	dev->header_ops		= &eth_header_ops;
	dev->type		= ARPHRD_ETHER;
	dev->hard_header_len 	= ETH_HLEN;
	dev->mtu		= ETH_DATA_LEN;
	dev->addr_len		= ETH_ALEN;
	dev->tx_queue_len	= 1000;	/* Ethernet wants good queues */
	dev->flags		= IFF_BROADCAST|IFF_MULTICAST;
	dev->priv_flags		|= IFF_TX_SKB_SHARING;

	memset(dev->broadcast, 0xFF, ETH_ALEN);
};
// 以太网驱动, 同一调用这个方法初始化, dev->header_ops->create 方法会把以太网头塞进去
// --------------------------------------------------
// end: link层
// --------------------------------------------------

// inet 的 tcp 协议方法
const struct proto_ops inet_stream_ops = {
	.family		   = PF_INET,
	.owner		   = THIS_MODULE,
	.release	   = inet_release,        // 系统调用 close 计数器为0的销毁方法
	.bind		   = inet_bind,
	.connect	   = inet_stream_connect, // 系统调用 connect  回调
	.socketpair	   = sock_no_socketpair,
	.accept		   = inet_accept,         // 系统调用 accept   回调
	.getname	   = inet_getname,
	.poll		   = tcp_poll,
	.ioctl		   = inet_ioctl,
	.listen		   = inet_listen,         // 系统调用 listen   回调
	.shutdown	   = inet_shutdown,       // 系统调用 shutdown 回调
	.setsockopt	   = sock_common_setsockopt,
	.getsockopt	   = sock_common_getsockopt,
	.sendmsg	   = inet_sendmsg,
	.recvmsg	   = inet_recvmsg,
	.mmap		   = sock_no_mmap,
	.sendpage	   = inet_sendpage,
	.splice_read	   = tcp_splice_read,
#ifdef CONFIG_COMPAT
	.compat_setsockopt = compat_sock_common_setsockopt,
	.compat_getsockopt = compat_sock_common_getsockopt,
	.compat_ioctl	   = inet_compat_ioctl,
#endif
};

const struct proto tcp_prot = {
	.name			= "TCP",
	.owner			= THIS_MODULE,
	.close			= tcp_close,
	.connect		= tcp_v4_connect,       // proto_ops->connect  的回调
	.disconnect		= tcp_disconnect,
	.accept			= inet_csk_accept,
	.ioctl			= tcp_ioctl,            // proto_ops->ioctl    的回调
	.init			= tcp_v4_init_sock,
	.destroy		= tcp_v4_destroy_sock,
	.shutdown		= tcp_shutdown,         // proto_ops->shutdown 的回调
	.setsockopt		= tcp_setsockopt,
	.getsockopt		= tcp_getsockopt,
	.recvmsg		= tcp_recvmsg,          // proto_ops->recvmsg  的回调
	.sendmsg		= tcp_sendmsg,          // proto_ops->sendmsg  的回调
	.sendpage		= tcp_sendpage,
	.backlog_rcv    = tcp_v4_do_rcv,
	.release_cb		= tcp_release_cb,
	.hash			= inet_hash,
	.unhash			= inet_unhash,
	.get_port		= inet_csk_get_port,
	.enter_memory_pressure	= tcp_enter_memory_pressure,
	.stream_memory_free	= tcp_stream_memory_free,
	.sockets_allocated	= &tcp_sockets_allocated,
	.orphan_count		= &tcp_orphan_count,
	.memory_allocated	= &tcp_memory_allocated,
	.memory_pressure	= &tcp_memory_pressure,
	.sysctl_mem		= sysctl_tcp_mem,
	.sysctl_wmem		= sysctl_tcp_wmem,
	.sysctl_rmem		= sysctl_tcp_rmem,
	.max_header		= MAX_TCP_HEADER,
	.obj_size		= sizeof(struct tcp_sock),
	.slab_flags		= SLAB_DESTROY_BY_RCU,
	.twsk_prot		= &tcp_timewait_sock_ops,
	.rsk_prot		= &tcp_request_sock_ops,
	.h.hashinfo		= &tcp_hashinfo,
	.no_autobind		= true,
#ifdef CONFIG_COMPAT
	.compat_setsockopt	= compat_tcp_setsockopt,
	.compat_getsockopt	= compat_tcp_getsockopt,
#endif
#ifdef CONFIG_MEMCG_KMEM
	.init_cgroup		= tcp_init_cgroup,
	.destroy_cgroup		= tcp_destroy_cgroup,
	.proto_cgroup		= tcp_proto_cgroup,
#endif
};