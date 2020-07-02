Network interfaces are the third standard class of Linux devices. (1. char; 2. block)

not in /dev/
not file_operations
Several hundred sockets can be multiplexed on the same physical interface.

块设备   只需要 响应 内核的请求
网络设备 还需要 响应 收包; 然后回传到协议栈
一系列 administrative tasks:
 setting addresses, modifying transmission parameters,
 maintaining  traffic  and  error  statistics

The  network  subsystem  of  the  Linux  kernel  is  designed  to  be  completely  protocol-independent.

drivers/net/loopback.c
[TOC]
example: plip.c e100.c loopback.c

## 网卡设备定义 struct netdevice
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
}

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

## 网卡设备方法 struct net_device_ops
```c++

```

## 网卡设备管理 ( 创建 / 注册 / 注销 / 释放 )
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
