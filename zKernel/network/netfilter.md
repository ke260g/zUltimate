其实netfliter是个大的框架，
在ipv4中对应的应用层工具是iptables，   有raw，filter，nat，mangle，security，5个table，
在bridge中对应的应用层工具是ebtables， 有broute，filter，nat，3个table，
在arp中对应的应用层工具是arptables,    有filter，1个table



白痴文
使用与实现 c代码例子 (需要点ko开发的前提知识)
https://blog.csdn.net/zhangskd/article/details/22678659
框架讲解中文版
https://blog.csdn.net/zqixiao_09/article/details/79057301

iptables对于链路层的arp包暂时无能为力
ebtables倒是可以过滤掉二层arp包

每个协议簇有5个HOOK点,
每个HOOK点有个list,里头是一堆回调
注册回调时, 要传入优先级, (我怀疑 注册时根据优先级插入list的位置)
优先级有宏, 每个协议簇的宏是独立的, 但是实际上就是-150~150的数字


```c
// 5个HOOK点
/* Bridge Hooks */
#define NF_BR_PRE_ROUTING	0 /* After promisc drops, checksum checks. */

#define NF_BR_LOCAL_IN		1 /* If the packet is destined for this box. */

#define NF_BR_FORWARD		2 /* If the packet is destined for another interface. */

#define NF_BR_LOCAL_OUT		3 /* Packets coming from a local process. */

#define NF_BR_POST_ROUTING	4 /* Packets about to hit the wire. */

// 注册/注销
int nf_register_hook(struct nf_hook_ops *reg);
void nf_unregister_hook(struct nf_hook_ops *reg);
// 注册后, reg中记录着list的节点, 用于在注销的时候 从链表中删除

// 回调函数有固定的返回值, 内核根据返回值对 数据包 进行后续操作
NF_ACCEPT
NF_DROP
NF_STOLEN
NF_QUEUE
NF_REPEAT
```