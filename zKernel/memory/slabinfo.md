cat /proc/slabinfo

## vm_area_struct
## inode_cache  shmem_inode_cache
一般来说是有内核泄露; 需要配合kmemleak; 排查
## nf_conntrack
/proc/net/nf_conntrack_*
/proc/sys/net/netfilter/nf_conntrack_*
/proc/net/ip_conntrack
> ospf/rip/vrrp/bfd等每发一个报文; 都会在内核中保留一个连接; 如果流量一直有; 那么这个连接一直保留