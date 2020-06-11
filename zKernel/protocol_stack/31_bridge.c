int netdev_rx_handler_register(struct net_device *dev,
			       rx_handler_func_t *rx_handler,
			       void *rx_handler_data)
{
	if (netdev_is_rx_handler_busy(dev))
		return -EBUSY;

	/* Note: rx_handler_data must be set before rx_handler */
	rcu_assign_pointer(dev->rx_handler_data, rx_handler_data);
	rcu_assign_pointer(dev->rx_handler, rx_handler);

	return 0;
}

int br_add_if(struct net_bridge *br, struct net_device *dev) {
    netdev_rx_handler_register(dev, br_handle_frame, p);

}


// brctl addif <bridge> <interface>;
// 实际上把 br_handle_frame 函数注册到  dev->rx_handler 中
netdev_rx_handler_register
br_add_if
add_del_if

// 收包时; 通过网桥转发
skb->dev->rx_handler(skb)
__netif_receive_skb_core
netif_receive_skb_core