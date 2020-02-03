int __udp4_lib_rcv(struct sk_buff *skb, struct udp_table *udptable,
		   int proto)
{
	struct sock *sk;
	struct udphdr *uh; // udp 协议头

    // ...

    // 校验码
	if (udp4_csum_init(skb, uh, proto))
		goto csum_error;

    // 处理 NF_HOOK 提前塞进来的socket
	sk = skb_steal_sock(skb);
	if (sk) {
		struct dst_entry *dst = skb_dst(skb);
		int ret;

		if (unlikely(sk->sk_rx_dst != dst))
			udp_sk_rx_dst_set(sk, dst);

		ret = udp_queue_rcv_skb(sk, skb); // udp收包入列
		sock_put(sk);                     // 销毁socket
		/* a return value > 0 means to resubmit the input, but
		 * it wants the return to be -protocol, or 0
		 */
		if (ret > 0)
			return -ret;
		return 0;
	} else {
        // UDP广播
		if (rt->rt_flags & (RTCF_BROADCAST|RTCF_MULTICAST)) 
			return __udp4_lib_mcast_deliver(net, skb, uh,
					saddr, daddr, udptable);
        // UDP单播, 通过 src-port&dst-port 查表, 找socket
		sk = __udp4_lib_lookup_skb(skb, uh->source, uh->dest, udptable);
	}

	if (sk != NULL) {
        // 走到这里 说明skb_steal_sock返回NULL
        //          且rt_flags不为广播/组播
        //          且__udp4_lib_lookup_skb返回非NULL
        // 一般逻辑 skb没有被提前塞入socket; 
        //          且通过udp表查到监听的socket
		ret = udp_queue_rcv_skb(sk, skb);   // udp收包入列
		sock_put(sk);                       // 销毁socket

		if (ret > 0)
			return -ret;
		return 0;
	}

	// ... xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)

	nf_reset(skb);

    // ... udp_lib_checksum_complete(skb)

	// ...

    // 走到这里, 说明传输层 失败
    // 1. 通过icmp 告知源主机
    // 2. 直接销毁 skb
	icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PORT_UNREACH, 0);
	kfree_skb(skb);

    // ...
}


int sock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
    // ...
	err = sk_filter(sk, skb); // BPF过滤器
    // ...
	return __sock_queue_rcv_skb(sk, skb);
}