# VRRP
Virtual Router Redundancy Protocol
provides for automatic assignment of available Internet Protocol (IP) routers to **participating hosts**

virtual routers: **abstract representation** of **multiple routers**
i.e. master and backup routers  --> acting as a group

The default gateway of a **participating host** is assigned to the **virtual router** instead of a **physical router**

If the **physical router** that is routing packets on behalf of the **virtual router** fails, **another physical router** is selected to automatically replace it. The **physical router** that is forwarding packets at any given time is called the **master router**.


一个二层网LAN (比如说是傻瓜交换机)
两个WAC, 或者两个路由,实现VRRP协议
三个IP  vip, vrrp-ip-masker, vrrp-ip-backup,
两个WAC  抢占vip, 即记录在傻瓜交换机中的"mac地址-ip"项

场景1:
如果backup机子 收到VIP网段的IP报文, 通过vrrp-ip-*通讯转给master机子, master机子作路由
场景2:
如果master机子挂掉了, 则, 无人跟backup抢占vip, backup机子成为vip对应的唯一机子,
此时backup机子作路由