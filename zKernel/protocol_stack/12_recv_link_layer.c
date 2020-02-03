

// ethernet 帧处理
__be16 eth_type_trans(struct sk_buff *skb, struct net_device *dev)
{
    // 求得mac_header偏移 skb->mac_header = skb->data - skb->head;
    skb_reset_mac_header(skb);
    // "剥掉" Link层
	skb_pull_inline(skb, ETH_HLEN);
    // 协议头 format::struct
	eth = eth_hdr(skb);
    // ...

    return eth->h_proto;
}

// 驱动层收包
static int rtl8139_rx(struct net_device *dev, struct rtl8139_private *tp,
		      int budget) {
    // 处理 DSA, TRAILER, LLC, IPX
    // 处理 skb->pkt_type = { PACKET_BROADCAST PACKET_MULTICAST PACKET_OTHERHOST }
    // eth_type_trans 中 "剥掉"Link层
    skb->protocol = eth_type_trans (skb, dev);

    // 收包
    netif_receive_skb (skb);
}

static int __netif_receive_skb_core(struct sk_buff *skb, bool pfmemalloc) {
    orig_dev = skb->dev; // 处理vlan的过程中, skb->dev是会改变的, 因此先保存下来
    // 求得network_header偏移 skb->network_header = skb->data - skb->head;
    skb_reset_network_header(skb);
 
    // 此处不是真正意义上的 关于TCP/UDP的传输层 
	if (!skb_transport_header_was_set(skb)) 
		skb_reset_transport_header(skb);

    // ...
    pt_prev = NULL;
another_round:
	skb->skb_iif = skb->dev->ifindex;

    // VLAN处理1: 剥掉VLAN
	if (skb->protocol == cpu_to_be16(ETH_P_8021Q) ||
	    skb->protocol == cpu_to_be16(ETH_P_8021AD)) {
		skb = skb_vlan_untag(skb);
		if (unlikely(!skb))
			goto out;
	}

    // 根据EtherType 获取网络层回调, 储存在pt_prev
    list_for_each_entry_rcu(ptype, &ptype_all, list) {
		if (!ptype->dev || ptype->dev == skb->dev) {
			if (pt_prev) // 已取得 网络层回调
				ret = deliver_skb(skb, pt_prev, orig_dev);
            // 未取得网络层回调, 则赋值
			pt_prev = ptype;
		}
	}

    // VLAN处理2: 
	if (vlan_tx_tag_present(skb)) {
		if (pt_prev) { // 之前已取得 网络层回调
			ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = NULL;
		}
		if (vlan_do_receive(&skb))
			goto another_round;
		else if (unlikely(!skb))
			goto out;
	}

    // VLAN处理3: vlan处理结束; 清除vlan处理标记
    if (unlikely(vlan_tx_tag_present(skb))) {
		if (vlan_tx_tag_get_id(skb))
			skb->pkt_type = PACKET_OTHERHOST;
		skb->vlan_tci = 0;
	}

    /* deliver only exact match when indicated */
	null_or_dev = deliver_exact ? skb->dev : NULL;
	type = skb->protocol; // IP协议, ARP协议
	list_for_each_entry_rcu(ptype,
			&ptype_base[ntohs(type) & PTYPE_HASH_MASK], list) {
		if (ptype->type == type &&
		    (ptype->dev == null_or_dev || ptype->dev == skb->dev ||
		     ptype->dev == orig_dev)) {
			if (pt_prev) // 之前已取得 网络层回调
				ret = deliver_skb(skb, pt_prev, orig_dev);
			pt_prev = ptype;
		}
	}

	if (pt_prev) {
		// ... 之前被标记为 orphans包处理
		ret = pt_prev->func(skb, skb->dev, pt_prev, orig_dev);
	} else {
drop: // 丢包处理
		atomic_long_inc(&skb->dev->rx_dropped);
		kfree_skb(skb);
	}
    // ...
}

// "剥掉" vlan
struct sk_buff *skb_vlan_untag(struct sk_buff *skb) {
    // 剥掉VLAN, 这里的"剥掉"其实是有特殊含义的, 
    // eth_type_trans 已经剥掉14 bytes, 即skb->data指向了 802.1Q的第三bytes
    // 这里实际"剥掉"了 802.1Q的第三 第四bytes, 以及以太网帧的EtherType
    if (unlikely(!pskb_may_pull(skb, VLAN_HLEN))) // #define VLAN_HLEN	4
		goto err_free;


    // VLAN帧处理后, 重置header偏移, 之前header偏移是基于无VLAN计算的
    skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);
	return skb;
}