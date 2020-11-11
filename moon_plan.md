1. perform first layer of kernel framework
2. after then hacking the network stack



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

http://www.brendangregg.com/linuxperf.html

/proc/loadavg 了解一下