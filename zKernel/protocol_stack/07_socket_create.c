SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)

sock_create(int family, int type, int protocol, struct socket **res);

int __sock_create(struct net *net, int family, int type, int protocol,  struct socket **res, int kern);
sock->type = type; // 传递 SOCK_STREAM
// pf 即 inet_family_ops
pf = rcu_dereference(net_families[family]); pf->create(net, sock, protocol, kern);
// inet_family_ops->create 指向 inet_create
int inet_create(struct net *net, struct socket *sock, int protocol, int kern) {
    struct sock *sk;
    // 在  inetsw[] 中找到 tcp 协议
    struct inet_protosw *answer;
	struct proto *answer_prot;
    list_for_each_entry_rcu(answer, &inetsw[sock->type], list) {
		err = 0;
		/* Check the non-wild match. */
		if (protocol == answer->protocol) {
			if (protocol != IPPROTO_IP)
				break;
		} else {
			/* Check for the two wild cases. */
			if (IPPROTO_IP == protocol) {
				protocol = answer->protocol;
				break;
			}
			if (IPPROTO_IP == answer->protocol)
				break;
		}
		err = -EPROTONOSUPPORT;
	}
	sock->ops = answer->ops;
	answer_prot = answer->prot;

    // 分配 struct sock; 其中 sk->sk_prot = answer_prot;
    sk = sk_alloc(net, PF_INET, GFP_KERNEL, answer_prot);

    // 初始化回调
	if (sk->sk_prot->init) {
		err = sk->sk_prot->init(sk);
    }
}
// sk->sk_prot 如果是tcp协议; 指向 tcp_prot
// sk->sk_prot 如果是udp协议; 指向 udp_prot
//
// 从而; 是的后续的调用 有以下层级关系
// fd >> socket >> sock >> inet >> udp/tcp
// 这些系统调用包括 
// socket层: connect bind listen accept send族 recv组 shutdown
// fd 层: close write read