[TOC]
星辰计划
master in linuxC, ipc-programming, network-programming, system-programming
wellin    kernel-module; network_device; interrupt; memory mapping & dma;
          netlink; bridge_forward; netfilter;

apt show nodejs-mozilla // 这个是 debian 下开源的 nodejs, 不需要手动从官网下 .deb

# 1. c++
https://www.cnblogs.com/heartchord/p/4476615.html (manual list)
对象模型 (候捷)
stl 源码 (候捷)

# 2. kernel
调度器哪里实现 多线程 有自己的栈空间??
硬中断, 实际上是 cpu 异常的一种类型
还有 取指令异常, 后者为数据异常, 等, 
内核 会有 SIGSEGV,  缺页, MMU访问权限异常
cgroups
coredump

内存系统: slabinfo

1. ldd3 原文
    http://lwn.net/Kernel/LDD3/
    http://lwn.net/images/pdf/LDD3/ldd3_pdf.tar.bz2

# 3. network
http://www.embeddedlinux.org.cn/linux_net/
协议栈的 scope 和 net_namespace 分别是什么意思 ??
https://zhuanlan.zhihu.com/p/22813775?refer=kls-software-arch-world  Linux性能优化12：网络IO的调度模型
https://www.cnblogs.com/listenerln/p/6846328.html 稍微清晰一点的 ip_route_input_slow 函数分析(其实也很鸡肋)
协议栈 qdisc gso

## 3.1 网卡对象
if_tun.h
if_tunnel.h
if_vlan.h
if_tap.h
if_link.h
if_ether.h
if_arp.h
inetdevice.h

## 3.2 arp
arp_process 中改代码; 实现学免费arp
```c++
if (inet_confirm_dst_addr(in_dev, sip)) {
    n = __neigh_lookup(&arp_tbl, &sip, dev, 1);
}
```

# 4. posix
/proc/loadavg 了解一下
http://www.brendangregg.com/linuxperf.html
mount -t proc proc /proc
linker and loaders, elf 文件, exec 族的底层实现
man 2 prctl

# 5. libuv
# 6. redis
# 7. tools
proccess: top / ps / strace / ltrace / netstat / tcpdump / iptable / wireshark
shell:    sed / awk / grep / sort / uniq; number-calculate; date-compare;
devel:    gdb / gcc / valgrind

