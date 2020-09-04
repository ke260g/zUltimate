1. perform first layer of kernel framework
2. after then hacking the network stack



# todo list
1. 什么是内存屏障; 为啥 kfifo / seqlock 调用 smp_wmb smp_rmb 就可以解决同步问题
    + 以及很多调用 wmb / rmb 的地方??
    + 目的在于能够 见到 wmb / rmb 的地方; 解释清楚为啥需要用到
2. tasklet   的实现
3. workqueue 的实现
4. generic interrupt framework



https://zhuanlan.zhihu.com/p/22813775?refer=kls-software-arch-world  Linux性能优化12：网络IO的调度模型

https://www.cnblogs.com/listenerln/p/6846328.html 稍微清晰一点的 ip_route_input_slow 函数分析(其实也很鸡肋)


arp_process 中改代码; 实现学免费arp
```c++
if (inet_confirm_dst_addr(in_dev, sip)) {
    n = __neigh_lookup(&arp_tbl, &sip, dev, 1);
}
```

建议研究:
if_tun.h
if_tunnel.h
if_vlan.h
if_tap.h
if_link.h
if_ether.h
if_arp.h
inetdevice.h

调度器哪里实现 多线程 有自己的栈空间??

硬中断, 实际上是 cpu 异常的一种类型
还有 取指令异常, 后者为数据异常, 等, 
内核 会有 SIGSEGV,  缺页, MMU访问权限异常

## 协议栈的 scope 和 net_namespace 分别是什么意思 ??


协议栈 qdisc gso