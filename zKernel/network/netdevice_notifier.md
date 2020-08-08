static int m_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
    struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	switch (event) {
	case NETDEV_REGISTER:
        printk("netdevice %s is registered\n", dev->name);
		break;
	case NETDEV_UNREGISTER:
        printk("netdevice %s is unregistered\n", dev->name);
		break;
	}
	return NOTIFY_DONE;
}

static struct notifier_block m_notifier_block = {
	.notifier_call	= m_device_event,
};

void knet_notifier_init(void)
{
    register_netdevice_notifier(&m_notifier_block);
}