参考 https://blog.csdn.net/zxorange321/article/details/75676063
参考 https://www.cs.unh.edu/cnrg/people/gherrin/linux-net.html#tth_sEc3.2.2
参考 https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/
1. 协议头数据结构
2. data_struct

struct msghdr 包含
  struct iovec

net_proto_family // 协议簇回调, 只有create方法  /* This is the 1st argument to socket(2) */
proto_ops // socket操作接口, socket -> inet_socket,
proto     // sock操作接口,   sock -> socket,
net_protocol // ip  层回调, 操作对象:sk_buff, 回调协议tcp/udp/icmp/igmp/sctp
packet_type  // link层回调, 操作对象:sk_buff, 回调协议ip/ip6/arp/rarp/bootp/pppoes

inet_protosw 包含
  proto
  proto_ops

socket 包含
  file
    file_operations
  sock (包含socket父级指针)
    sock_common
  proto_ops

sock   引用
  inet_sock
    sctp_sock
    udp_sock
    raw_sock
    inet_connection_sock
      tcp_sock
  

