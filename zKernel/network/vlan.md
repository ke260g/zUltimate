[TOC]
主目录 = ./net/8021q/

## vlan 关键入口
### 收包逻辑: __netif_receive_skb_core > vlan_do_receive > __netif_receive_skb_core
```c++
if (vlan_tx_tag_present(skb)) {
    if (pt_prev) {
        ret = deliver_skb(skb, pt_prev, orig_dev);
        pt_prev = NULL;
    }
    if (vlan_do_receive(&skb))
        goto another_round; // 这里地方 skb->dev 已经重载了
    else if (unlikely(!skb))
        goto out;
}
```
### 发包逻辑: vlan_dev_hard_start_xmit
```c++
    // 一个 vlan只能有一个子设备; 通常就是桥(内核最初的设计)
    skb->dev = vlan->real_dev; // 重定向到vlan子设备
    len = skb->len;
    if (unlikely(netpoll_tx_running(dev)))
        return vlan_netpoll_send_skb(vlan, skb);

    ret = dev_queue_xmit(skb);
```
### vlan注册: vlan_ioctl_handler > register_vlan_device
### 设备方法: struct net_device_ops vlan_netdev_ops

### 加tag逻辑: dev_queue_xmit > dev_hard_start_xmit
```c++
    if (vlan_tx_tag_present(skb) &&
        !vlan_hw_offload_capable(features, skb->vlan_proto)) {
        skb = __vlan_put_tag(skb, skb->vlan_proto,
                        vlan_tx_tag_get(skb));
        if (unlikely(!skb))
            goto out;

        skb->vlan_tci = 0;
    }
```
### 剥tag逻辑: __netif_receive_skb_core
```c++
	if (skb->protocol == cpu_to_be16(ETH_P_8021Q) ||
	    skb->protocol == cpu_to_be16(ETH_P_8021AD)) {
		skb = skb_vlan_untag(skb);
		if (unlikely(!skb))
			goto out;
	}
```