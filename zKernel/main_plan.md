ldd3 at first

## 1. protocol-stack:
 level 1: preview trace udp
 level 2: Understanding Linux Network Internals ( vlan bridge )
 level 3: zero copy; fib_lookup
 + ip 的拆包组包
 + netfilter 研究方向:
    1. 在内核中搜 struct nf_hook_os 的引用
    2. 在内核中搜 NF_HOOK 的引用了解各个钩子节点挂了那些钩子
    3. 在内核中搜 NF_CT_STAT_INC 了解具体的统计计数 用于排查包流向

## 2. memory:
 slabinfo: trace the slabinfo entry in kernel, create by proc_create()
 kmemleak: have a look on kmemleak dump file
 https://linux-kernel-labs.github.io/refs/heads/master memmory section

## 3. more = ( lkd3 ulk )
