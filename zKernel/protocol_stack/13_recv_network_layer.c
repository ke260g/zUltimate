// 主要源码 ip_input.c，ip_forward.c，ip_output.c，ip_fragment.c, route.c

int ip_rcv(struct sk_buff *skb, struct net_device *dev, struct packet_type *pt, struct net_device *orig_dev) { 
    // PACKET_OTHERHOST 非本机, drop
    // pkt_type 在 eth_type_trans 函数中处理
	if (skb->pkt_type == PACKET_OTHERHOST)
		goto drop;

    // 共享报文 则clone一份
    skb = skb_share_check(skb, GFP_ATOMIC);

    if (!pskb_may_pull(skb, sizeof(struct iphdr))) // 检查是否有足够的位置 "剥掉"
		goto inhdr_error;
    // network层; skb->head + skb->network_header;
    iph = ip_hdr(skb);
    // ...
    // iph->ihl 为IP协议头的 4bits长度, indicate header的字节数
	if (!pskb_may_pull(skb, iph->ihl*4)) // 检查是否有足够的位置 "剥掉"
		goto inhdr_error;
	iph = ip_hdr(skb);

    if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl))) // 校验码
		goto csum_error;

    // "长度" 检查, iph->tot_len 对应IP协议头indicate整个报文的长度
    len = ntohs(iph->tot_len);
	if (skb->len < len) { // skb->len 长度较小, 该报文被截断了
		goto drop;
	} else if (len < (iph->ihl*4)) // total长度 < header长度, 则IP报文错误
		goto inhdr_error;
        
    // 本质操作: 设置skb->len, skb->tail
    // skb->len = len;
	// skb->tail = skb->data + len;
    pskb_trim_rcsum(skb, len);

    // 真实意义上 TCP/UDP 的传输层
    skb->transport_header = skb->network_header + iph->ihl*4;

	/* Remove any debris in the socket control block */
	memset(IPCB(skb), 0, sizeof(struct inet_skb_parm)); // TODO: 看不懂

    // 网络层处理结束, 标记为孤儿, 使得 Link层中 后续的处理为drop
	skb_orphan(skb);
	return NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING, skb, dev, NULL,
		       ip_rcv_finish);
}

static int ip_rcv_finish(struct sk_buff *skb)
{
    // early_demux ...

    // ip_route_input_noref
    // 查路由表, 处理多播逻辑, 赋值skb->_skb_refdst->input
	if (!skb_dst(skb)) {
		err = ip_route_input_noref(skb, iph->daddr, iph->saddr,
					   iph->tos, skb->dev);
		if (unlikely(err))
			goto drop_error;
	}

    // ip_route_classid ...
    // rtable 路由 统计 ...

    // 调用 skb->_skb_refdst->input 方法
    // 如果通过 netfilter::LOCAL_IN, 则调用 ip_local_deliver()
    // 如果通过 netfilter::FORWARD,  则调用 ip_forward()
	return dst_input(skb);
}

// 单播 / 多播区分
int ip_route_input_noref(struct sk_buff *skb, __be32 daddr, __be32 saddr,
			 u8 tos, struct net_device *dev)
{
    // 多播处理
    if (ipv4_is_multicast(daddr)) {
        int res = ip_route_input_mc(skb, daddr, saddr,
                tos, dev, our);
        return res;
    }
    // 单播处理
    res = ip_route_input_slow(skb, daddr, saddr, tos, dev);
    return res;
}

// 
// 业务输出: 6种结果 { local_input brd_input no_route martian_source martian_destination forward }
//           martian_source martian_destination RFC1812 非法地址报文要要求统计
//           no_route  本质同于 local_input
//           brd_input 先预处理(源IP校验, 打上广播标记, 统计) 再 local_input
//           local_input 调用 rt_dst_alloc(flags | RTCF_LOCAL); 中赋值 dst.input = ip_local_deliver;
//           forward   如果报文不属于上述结果, 则默认走转发逻辑
//                     ip_mkroute_input() 中赋值 dst.input = ip_forward;
static int ip_route_input_slow(struct sk_buff *skb, __be32 daddr, __be32 saddr,
			       u8 tos, struct net_device *dev)
{
    struct fib_result res;
    struct flowi4	fl4;
    
    // 1. 转发预处理: 即可fib_lookup无法处理的
    // multicast 指 0xe(1110) 开头的ip地址
    // lbcast    指 255.255.255.255 开头的ip地址
    if (ipv4_is_multicast(saddr) || ipv4_is_lbcast(saddr))    // 1.1 src-addr 为组播/广播, 则非法源
		goto martian_source;
    if (ipv4_is_lbcast(daddr) || (saddr == 0 && daddr == 0))  // dst-ip 为广播; src-ip == dst-ip == 0; 则为广播包
		goto brd_input;
    if (ipv4_is_zeronet(saddr))                               // src-ip == 0, 但 (dst-ip != 0)
		goto martian_source;
	if (ipv4_is_zeronet(daddr))                               // dst-ip == 0, 但 (src-ip != 0)
		goto martian_destination;
    if (ipv4_is_loopback(daddr)) {                            // 不理解loopback地址的处理?
		if (!IN_DEV_NET_ROUTE_LOCALNET(in_dev, net))
			goto martian_destination;
	} else if (ipv4_is_loopback(saddr)) {
		if (!IN_DEV_NET_ROUTE_LOCALNET(in_dev, net))
			goto martian_source;
	}
    
    // 2. 查询转发表, 输出结果存储到res变量
    fib_lookup(net, &fl4, &res);
    
    // 3. 根据转发结果 分发处理逻辑
    if (res.type == RTN_BROADCAST)
		goto brd_input;
	if (res.type == RTN_LOCAL) {
		err = fib_validate_source(skb, saddr, daddr, tos,
					  0, dev, in_dev, &itag);
		if (err < 0)
			goto martian_source_keep_err;
		goto local_input;
	}
	if (!IN_DEV_FORWARD(in_dev)) {
		err = -EHOSTUNREACH;
		goto no_route;
	}
	if (res.type != RTN_UNICAST)
		goto martian_destination;

    // 4. 后续是一系列的相应处理
    // 走到这里, res.type == RTN_UNICAST
    err = ip_mkroute_input(skb, &res, &fl4, in_dev, daddr, saddr, tos);
out:	return err;

brd_input:
    // ...
	if (!ipv4_is_zeronet(saddr)) {
        // a. 走到此处; 说明brd_input判定时, daddr为255.255.255.255
        // b. 如果 daddr == 255.255.255.255; saddr != 0 则需要在 fib中校验合法性
        // c. 如果 daddr == 255.255.255.255; saddr == 0 则不检查 src-ip
        //    (这种报文 如果需要回程通讯, 源地址我猜是在应用层定义的)
		err = fib_validate_source(skb, saddr, 0, tos, 0, dev,
					  in_dev, &itag);
		if (err < 0)
			goto martian_source_keep_err;
	}
    // ...

local_input:
	do_cache = false;
    // ... route cache logic

    // 由于 flags | RTCF_LOCAL
    // 所以 赋值 ip_local_deliver; 从而进入 netfilter::LOCAL_IN
	rth = rt_dst_alloc(net->loopback_dev, flags | RTCF_LOCAL, res.type,
			   IN_DEV_CONF_GET(in_dev, NOPOLICY), false, do_cache);
    // ...
	rth->dst.output= ip_rt_bug; // LOCAL_IN 后不应该跑到output; 如果有, 则是bug
    // ... ip route classid
	rth->rt_is_input = 1;

	RT_CACHE_STAT_INC(in_slow_tot);
	if (res.type == RTN_UNREACHABLE) {
		rth->dst.input= ip_error;
		rth->dst.error= -err;
		rth->rt_flags 	&= ~RTCF_LOCAL;
	}
    
    // ... route cache logic
	
    skb_dst_set(skb, &rth->dst);
	err = 0;
	goto out;

no_route:
	RT_CACHE_STAT_INC(in_no_route);
	res.type = RTN_UNREACHABLE;
	res.fi = NULL;
	if (err == -ESRCH)
		err = -ENETUNREACH;
	goto local_input;
}

int ip_local_deliver(struct sk_buff *skb)
{
    // ip 组包 分包
	if (ip_is_fragment(ip_hdr(skb))) {
		if (ip_defrag(skb, IP_DEFRAG_LOCAL_DELIVER))
			return 0;
	}

	return NF_HOOK(NFPROTO_IPV4, NF_INET_LOCAL_IN, skb, skb->dev, NULL,
		       ip_local_deliver_finish);
}

static int ip_local_deliver_finish(struct sk_buff *skb)
{
    // ... 剥掉sk_buff的网络层
	{
		int protocol = ip_hdr(skb)->protocol;            // 获取 传输层协议
		const struct net_protocol *ipprot;
		int raw;

	resubmit:
		raw = raw_local_deliver(skb, protocol);          // 分发到raw_socket中

		ipprot = rcu_dereference(inet_protos[protocol]); // 获取传输层回调
		if (ipprot != NULL) {
            // ... xfrm4_policy_check 
			ret = ipprot->handler(skb);
			if (ret < 0) {
				protocol = -ret;
				goto resubmit;
			}
			// ...
		} else {
			if (!raw) {   // 走到这里 说明 无下一层协议回调 and 无raw_socket
                // icmp_send 返回给发送方
                // 销毁skb; skb生命周期结束
				if (xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
                    // ...
					icmp_send(skb, ICMP_DEST_UNREACH, ICMP_PROT_UNREACH, 0);
				}
				kfree_skb(skb);
			} else {      // 走到这里 说明 无下一层协议回调 and 有raw_socket
                // skb->users 引用数为1则销毁; 引用数非1则减1
				consume_skb(skb);
			}
		}
	}   
}