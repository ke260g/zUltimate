[TOC]
# 1. 对象
## 1.1 网卡设备定义 struct netdevice
```c++
struct net_device {
    char name[IFNAMSIZ];     /* 网卡名称 */
    unsigned long state;     /* 不直接操作; 使用标准 utiliy 设置 */

    /* 网卡硬件信息, 可能驱动不维护, 直接维护 priv 即可 */
    unsigned long mem_end;   /* shared mem end */
    unsigned long mem_start; /* shared mem start */
    unsigned long base_addr; /* device I/O address */
    unsigned char irq;       /* 中断计数 */

    /* 协议栈需要了解的 硬件信息 */
    unsigned int mtu;        /* interface MTU value */
    unsigned short type;     /* interface hardware type */ 
                             /* 用于邻居系统; 一般写 ARPHRD_ETHER; ether_setup已封装 */
                             /* 定义在 <uapi/linux/if_arp.h> ARPHRD_* */
    unsigned short hard_header_len; /* hardware hdr length */
    unsigned long tx_queue_len; /* Max frames per queue allowed */
                                /* 协议栈发包计数; 超过了则 ENOBUFS */

    /* MAC 地址 */
    unsigned char addr_len;
    unsigned char broadcast[MAX_ADDR_LEN];
    unsigned char dev_addr[MAX_ADDR_LEN];

    unsigned int flags; /* 定义在 <uapi/linux/if.h> IFF_* */
    netdev_features_t features; /* 定义在 <linux/netdev_features.h> NETIF_F_* */

    struct netdev_hw_addr_list mc; /* Multicast mac addresses */ /* 组播表 */

    struct net_device_stats	stats; /* 统计信息集合 */
    atomic_long_t rx_dropped; /* 仅协议栈 可写; 驱动层使用 @stats */
    atomic_long_t tx_dropped; /* 仅协议栈 可写; 驱动层使用 @stats */
};

/* 常用 IFF_* 标记 */
#define IFF_UP        // 驱动层只读; 只能由内核接口设置 
#define IFF_BROADCAST // 是否 收发 广播报文
#define IFF_MULTICAST // 是否 收发 组播报文
#define IFF_LOOPBACK  // 是否 回环设备; 协议栈检查该标记 执行业务
#define IFF_NOARP     // 是否 禁用ARP
#define IFF_PROMISC   // 是否 混杂模式; 协议栈检查该标记 执行业务; tcpdump拉起该标记

/* 常用 NETIF_F_* 标记 */
#define NETIF_F_IP_CSUM // 指示 协议栈发包不计算  IP CSUM; 驱动or硬件计算
#define NETIF_F_NO_CSUM // 指示 协议栈收发包都不计算 CSUM;
#define NETIF_F_HW_CSUM // 指示 协议栈发包不计算 任何CSUM(TCP/UDP/IP); 驱动or硬件计算
#define NETIF_F_HW_VLAN_TX
#define NETIF_F_HW_VLAN_RX
#define NETIF_F_HW_VLAN_FILTER
#define NETIF_F_VLAN_CHALLENGED
#define NETIF_F_TSO      // 指示 协议栈网卡自带 TCP segmentation offloading 功能
```

## 1.2 网卡设备方法 struct net_device_ops
```c++
struct net_device_ops {
    /* 基础回调 */
    int  (*ndo_init)(struct net_device *dev);    // register_netdev 回调; 更多地在alloc_netdev的setup初始化
    void (*ndo_uninit)(struct net_device *dev);  // unregister_netdev 回调; 一般不用; 销毁先由驱动 反初始化
    int  (*ndo_open)(struct net_device *dev);    // ifconfig $net_device up   回调; SIOCSIFFLAGS + IFF_UP
    int  (*ndo_stop)(struct net_device *dev);    // ifconfig $net_device down 回调; SIOCSIFFLAGS + IFF_DOWN
    netdev_tx_t  (*ndo_start_xmit)(struct sk_buff *skb, struct net_device *dev); // dev_queue_xmit 发包回调
    void (*ndo_set_rx_mode)(struct net_device *dev); // 设置 broadcast, multicast 的回调
    void (*ndo_set_mac_address)(struct net_device *dev, void *addr); // 设置 mac 地址回调; 必须非空; 否则mac无法设置
    int  (*ndo_validate_addr)(struct net_device *dev); // 检查 mac地址 是否合法; 必须非空; 一般用 eth_validate_addr
    int  (*ndo_change_mtu)(struct net_device *dev, int new_mtu); // 必须非空; 设置 mtu 回调
    int  (*ndo_do_ioctl)(struct net_device *dev, struct ifreq *ifr, int cmd); // 非标准的 ioctl 回调; 可以为空

    struct net_device_stats* (*ndo_get_stats)(struct net_device *dev); // 统计信息 回调; 驱动维护 net_device_stats 对象
    /* 更多方法; 可以为空 */
    int	watchdog_timeo;   // dev_watchdog 的超时间隔
    void (*ndo_tx_timeout) (struct net_device *dev); // 驱动层 发包超时回调;
                                                     // 驱动 dev->_tx[] 为 tx_queue; 其中有 trans_start 发包时间记录
                                                     // register_netdev 注册了定时器 dev_watchdog 检查 trans_start
                                                     // ndo_start_xmit 中; 必须更新 dev->_tx[]->trans_start 
                                                     // dev_watchdog 检查 出来超时后; 调用 ndo_tx_timeout 回调
                                                     // 调用 netif_trans_update 更新 dev->_tx[]->trans_start
#ifdef CONFIG_NET_POLL_CONTROLLER
    void (*ndo_poll_controller)(struct net_device *dev);
    int  (*ndo_netpoll_setup)(struct net_device *dev, struct netpoll_info *info);
    void (*ndo_netpoll_cleanup)(struct net_device *dev);
#endif
    /* neigh 方法 */
    int  (*ndo_neigh_construct)(struct net_device *dev, struct neighbour *n);
    void (*ndo_neigh_destroy)(struct net_device *dev, struct neighbour *n);

    /* fdb  方法 */
    int (*ndo_fdb_add)(struct ndmsg *ndm, struct nlattr *tb[], struct net_device *dev,
                        const unsigned char *addr, u16 vid, u16 flags);
    int (*ndo_fdb_del)(struct ndmsg *ndm, struct nlattr *tb[], struct net_device *dev,
                        const unsigned char *addr, u16 vid);
    int (*ndo_fdb_dump)(struct sk_buff *skb, struct netlink_callback *cb,
                        struct net_device *dev, struct net_device *filter_dev, int *idx);
}
```

## 1.3 网卡收发统计 struct net_device_stats
```c++
    /* 正常收发计数 */
    unsigned long rx_packets;
    unsigned long tx_packets;
    unsigned long rx_bytes;
    unsigned long tx_bytes;
    /* 异常收发计数 */
    unsigned long rx_errors;
    unsigned long tx_errors;
    unsigned long rx_dropped;
    unsigned long tx_dropped;
    /* 其他 */
    unsigned long multicast;  // 组播报文
    unsigned long collisions; // ??
}
```

# 2. 方法
## 2.1 网卡缓存队列 ( start / stop / wake / disable )
```c++
void netif_start_queue(struct net_device *dev);
void netif_stop_queue(struct net_device *dev);
void netif_wake_queue(struct net_device *dev); // start_queue; 且唤醒 协议栈 重新发送阻塞的 skb
void netif_tx_disable(struct net_device *dev); // 确保 所有CPU 都 netif_stop_queue

int ndo_open(struct net_device *dev) { // ifconfig $interface up
    // ... resources preparation
    netif_start_queue(dev); // open 方法尾 必须调用
}
int ndo_stop(struct net_device *dev) { // ifconfig $interface down
    netif_stop_queue(dev);  // stop 方法头 必须调用
    // ... resources release
}
int hardware_resource_busy_callback() { // 硬件资源 满了; 指示协议栈不可发包
    netif_stop_queue(dev);
}
int hardware_resource_free_callback() { // 硬件资源 充足; 指示协议栈可以发包
    netif_wake_queue(dev); // <--- 此处必须用 wake 而不是 start 方法
}
```

## 2.2 网卡硬件状态 ( linkup / linkdown )
```c++
// 硬件 linkup / linkdown
void netif_carrier_off(struct net_device *dev); // set linkup
void netif_carrier_on(struct net_device *dev);  // set linkdown
bool netif_carrier_ok(const struct net_device *dev); // get

int hardware_linkup_callback() {
    netif_carrier_on(dev);
}
int hardware_linkdown_callbak() {
    netif_carrier_off(dev);
}
```

## 2.3 网卡设备管理 ( 创建 / 注册 / 注销 / 释放 )
```c++
struct net_device;

// 0. 实现构造函数
void foobar_interface_setup(struct net_device *);
///////////////////////////////////////////////////

// 1. 创建 ========================================
struct net_device *alloc_netdev(int sizeof_priv,
    const char *name, void (*setup)(struct net_device *));
// 封装: name = eth%d, setup = ether_setup,
struct net_device *alloc_etherdev(int sizeof_priv);
///////////////////////////////////////////////////

// 2. 注册 ========================================
int register_netdev(struct net_device *dev);
///////////////////////////////////////////////////

// 3. 注销 ========================================
void unregister_netdev(struct net_device *dev);
///////////////////////////////////////////////////

// 4. 释放 ========================================
void unregister_netdev(struct net_device *dev);
///////////////////////////////////////////////////
```



# 3. 使用
## 3.1 网卡收包逻辑
1. 关掉硬件中断
2. dev_alloc_skb
3. 从 dma 拷贝到 skb
4. skb_put(sbk, len)
5. 设置 skb->dev 收包interface
6. 设置 skb->protocol = eth_type_trans(skb, dev);
7. 设置 skb->->ip_summed = CHECKSUM_XXX;
8. stats.rx_packets++;
9. stats.rx_bytes += skb->len
10. netif_receive_skb
11. 重新拉起中断

## 3.2 网卡发包逻辑
1. dev_queue_xmit
2. ndo_hard_xmit 回调
3. stats.tx_packets++
4. stats.tx_bytes += skb->len
5. skb copyto dma
6. enable interrupt
8. trigger hardware send packet
9. tx interrupt callback
10. disable interrupt
11. handle resouces-management and statistic issues

## 3.3 网卡硬件poll
+ 本质是为了解决 网卡大量收包 导致的频繁中断问题
+ 但是, 在收包较少的场景中, 反而会影响性能(poll的本质耗时)
### 3.3.1 步骤
+ 实现 net_device_ops->ndo_poll_controller
1. 关掉硬件中断
2. 调用中断回调 (该中断回调一般 等同收发包的中断回调)
3. 拉起硬件中断
### 3.3.2 策略
+ 查找 ndo_poll_controller 在内核协议栈中的调用关系

## 3.4 网卡零拷贝 shinfo
```c++
struct sk_buff {
    shinfo(struct sk_buff *skb);
    unsigned int shinfo(skb)->nr_frags;
    skb_frag_t shinfo(skb)->frags;
}; // zero-copy 矢量DMA的SKB逻辑
```

# 4. 例子
example: plip.c e100.c loopback.c