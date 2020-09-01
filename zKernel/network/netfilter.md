[TOC]
其实netfliter是个大的框架，
在ipv4中对应的应用层工具是iptables，   有raw，filter，nat，mangle，security，5个table，
在bridge中对应的应用层工具是ebtables， 有broute，filter，nat，3个table，
在arp中对应的应用层工具是arptables,    有filter，1个table


白痴文
使用与实现 c代码例子 (需要点ko开发的前提知识)
https://blog.csdn.net/zhangskd/article/details/22678659
框架讲解中文版
https://blog.csdn.net/zqixiao_09/article/details/79057301


可以通过分析  NF_HOOK 各个钩子点在协议栈的位置; 以及各个钩子点的 结束函数; 把协议栈串联起来;

# 相关文件
net/ipv4/netfilter.c
net/ipv6/netfilter.c

net/core/filter.c
net/netfilter/core.c

net/ipv4/filter/iptable_filter.c
net/ipv4/filter/ebtable_filter.c
net/ipv4/filter/arptable_filter.c
net/ipv4/filter/ip6table_filter.c


连接跟踪模块      conntrack net/netfilter/nf_conntrack_*.c
网络地址转换模块  nat       
数据报修改模块    mangle    net/ipv4/netfilter/*table_mangle.c

# netfilter 框架
1. netfilter 的框架本质有3个部分
    1. hook表 定义; 三维表 `表[协议簇][hook点][hook函数]`
    2. 回调函数 及其注册方法
    3. hook表 遍历; 协议帧的协议处理 在各个hook点 遍历相应的 hook函数
2. 实际上; hook点的生效在于 协议帧的各个部分 遍历hook函数; 如果不遍历, 则hook点无意义
## hook表 定义
1. 内核更新有点快; 这个表不好找; 建议搜 `NF_MAX_HOOKS`
2. linux-3.16.78
```c++
struct list_head nf_hooks[NFPROTO_NUMPROTO][NF_MAX_HOOKS] __read_mostly; // 二维数组 + 一维链表】
struct nf_hook_ops { // 一个hook函数的 存储对象; 用链表连接
	struct list_head list;
	nf_hookfn	*hook;
	struct module	*owner;
	void		*priv; // hook函数本身的上下文
	u_int8_t	pf;    // 协议簇
	unsigned int	hooknum; // hook点
	int		priority;  // 优先级
};
typedef unsigned int nf_hookfn(const struct nf_hook_ops *ops,
			       struct sk_buff *skb,
			       const struct net_device *in,
			       const struct net_device *out,
			       int (*okfn)(struct sk_buff *));
// netfilter 初始化
// 1. 调用 register_pernet_subsys 往 net_namespace 表注册一个节点 `struct net *net`
// 2. 初始化 netfilter 框架; 其中初始化 hook 表
int __init netfilter_init(void); 
```
3. linux-4.14.76
```c++
// hook 表
struct net { // pernet_list 中一个节点
    struct netns_nf nf;
};
struct netns_nf {
    struct nf_hook_entries *hooks[NFPROTO_NUMPROTO][NF_MAX_HOOKS]; // hook表本身
};
struct nf_hook_entries {   // 一个hook点下的 hook函数; 动态数组的方式存储
	u16				num_hook_entries;
	struct nf_hook_entry		hooks[];
};
struct nf_hook_entry {     // 一个hook函数的 存储对象; priv 作为hook函数的上下文
	nf_hookfn			*hook;
	void				*priv;
};
typedef unsigned int nf_hookfn(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);

// net/core/net_namespace.c  net_namespace 表
static LIST_HEAD(pernet_list);

// netfilter 初始化
// 1. 调用 register_pernet_subsys 往 net_namespace 表注册一个节点 `struct net *net`
// 2. 调用 netfilter_net_init     初始化 netfilter 框架; 其中初始化 hook 表
int __init netfilter_init(void); 
```

## hook表 遍历
1. 遍历入口函数 NF_HOOK;
2. 直接返回 NF_HOOK_THRESH; 
3. NF_HOOK_THRESH a) 调用 nf_hook_thresh > nf_hook_slow 遍历 hook表中hook点的函数
4. NF_HOOK_THRESH b) 遍历结束如果包 还需要往下执行; 则调用ok函数 okfn
```c++
inline int NF_HOOK_THRESH(uint8_t pf, unsigned int hook, struct sk_buff *skb,
	       struct net_device *in, struct net_device *out,
	       int (*okfn)(struct sk_buff *), int thresh)
{
	int ret = nf_hook_thresh(pf, hook, skb, in, out, okfn, thresh);  // 遍历链表 hook函数
	if (ret == 1)
		ret = okfn(skb);                                             // ok函数; 结束函数
	return ret;
}

// 已知的HOOK表 遍历点
NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING);
NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN);
NF_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD);
NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_OUT);
NF_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING);

NF_HOOK(NFPROTO_IPV4, NF_INET_PRE_ROUTING);
NF_HOOK(NFPROTO_IPV4, NF_INET_LOCAL_IN);
NF_HOOK(NFPROTO_IPV4, NF_INET_FORWARD);
NF_HOOK(NFPROTO_IPV4, NF_INET_LOCAL_IN);
NF_HOOK(NFPROTO_IPV4, NF_INET_POST_ROUTING);

NF_HOOK(NFPROTO_ARP, NF_ARP_IN);
NF_HOOK(NFPROTO_ARP, NF_ARP_FORWARD);
NF_HOOK(NFPROTO_ARP, NF_ARP_OUT);
```

## hook 函数 注册销毁
1. linux-3.16.78
```c++
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
2. linux-4.14.76
```c++
int nf_register_net_hook(struct net *net, const struct nf_hook_ops *ops);
void nf_unregister_net_hook(struct net *net, const struct nf_hook_ops *ops);
```