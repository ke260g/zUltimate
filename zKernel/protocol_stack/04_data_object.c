// --------------------------------------------------
// begin: socket层 >> link层:  famliy > socket-type > ip-protocol
// --------------------------------------------------
static const struct net_proto_family svc_family_ops;
static const struct net_proto_family inet_family_ops;
static const struct net_proto_family inet6_family_ops;
static const struct net_proto_family *net_families[NPROTO];

struct proto tcp_prot;
struct proto udp_prot;
struct proto raw_prot;
struct proto ping_prot;

const struct proto_ops inet_dgram_ops;
const struct proto_ops inet_stream_ops;
static const struct proto_ops inet_sockraw_ops;

/* Upon startup we insert all the elements in inetsw_array[] into
 * the linked list inetsw.
 */
static struct inet_protosw inetsw_array[] = {
    { SOCK_STREAM, IPPROTO_TCP,  &tcp_prot,  &inet_stream_ops,  },
    { SOCK_DGRAM,  IPPROTO_UDP,  &udp_prot,  &inet_dgram_ops,   },
    { SOCK_DGRAM,  IPPROTO_ICMP, &ping_prot, &inet_sockraw_ops, },
    { SOCK_RAW,    IPPROTO_IP,   &raw_prot,  &inet_sockraw_ops, },
};
// inet_sw[] inet_protosw->type, 即socket类型, 作为数组索引, 
// 每个数组成员中都有list, list成员对应一种层协议(ping/raw/udp/tcp)
// inet_register_protosw() 把inet_protosw对象插入inetsw
// inet_init 遍历inetsw_array的每个成员, 调用inet_register_protosw
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