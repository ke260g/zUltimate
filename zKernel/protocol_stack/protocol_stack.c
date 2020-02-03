参考 https://blog.csdn.net/zxorange321/article/details/75676063
参考 https://www.cs.unh.edu/cnrg/people/gherrin/linux-net.html#tth_sEc3.2.2
参考 https://www.saminiir.com/lets-code-tcp-ip-stack-1-ethernet-arp/

学习指南
1. 各种回调、对象 及其容器
2. 内核网络协议栈初始化过程
3. socket 创建过程
4. 发包过程 write
5. 收包过程 硬件中断 > ip_rcv > tcp_rcv/udp_rcv > read
6. udp 收发包
7. tcp 收发包
8. arp 收发包
+  收/发包过程概览(各层关键函数及其流程):
  + system call interface
  + protocol agnostic interface
  + network protocols
  + device agnostic interface
  + device drivers

遗留问题:
__netif_receive_skb_core() 中 packet_all, pt_prev 的处理是什么意思?
__netif_receive_skb_core() vlan的处理业务 实际中 应该是怎么样的?
IP_INC_STATS_BH() IP_ADD_STATS_BH() 属于什么业务??
pskb_may_pull()            该逻辑有些复杂, 看不懂
SDR messages? route cache? route cache entries? route cache reject entries?
ip_route_input_slow()      中 loopback 地址处理

net_proto_family // 协议簇回调, 只有create方法  /* This is the 1st argument to socket(2) */
proto_ops        // socket操作接口, socket -> inet_socket, /* This is the 2nd argument to socket(2) */
proto            // sock操作接口,   sock -> socket,
net_protocol     // ip  层回调, 操作对象:sk_buff, 回调协议tcp/udp/icmp/igmp/sctp
packet_type      // link层回调, 操作对象:sk_buff, 回调协议ip/ip6/arp/rarp/bootp/pppoes
napi_struct      // 收包控制
sk_buff          // 描述 一个报文的所有信息
sk_buff_head     // 内核缓存 sk_buff 的队列
net_device       // 描述 一个网络设备
softnet_data     // 描述一个CPU的收包队列

有哪些对象? 有哪些容器? 容器与对象、容器与容器的包含关系?
对象原型的包含关系

msghdr { iovec }
socket {
  file { file_operations { read write } }
  proto_ops { ioctl bind accept }
  sock { 
    sock_common { proto { ioctl bind accept } }
    sk_buff_head::sk_write_queue
    sk_buff_head::sk_receive_queue
  }
}

tcp_sock { inet_connection_sock { inet_sock { sock { sock_common } } } }
udp_sock { inet_sock { sock { sock_common } }  }

inet_protosw { proto_ops proto }

softnet_data {
  sk_buff_head::input_pkt_queue
  napi_struct {
    sk_buff {
      sock
      net_device
      sk_buff_data_t::transport_header
      sk_buff_data_t::network_header
      sk_buff_data_t::mac_header
    }
    poll
  }
}

packet_type {
   net_device
   func
}

/**
 * sk_buff 关键的成员
 * @vlan_tci: vlan tag control information
 *
 * @len: Length of actual data                 一层协议的数据总长度 包括协议的 header+buffer
 * @data_len: Data length
 * @mac_len: Length of link layer header
 * @hdr_len: writable header length of cloned skb
 *
 * @sk: Socket we are owned by
 * @_skb_refdst: destination entry (with norefcount bit)  TODO: 什么意思?
 * 
 * 
 * @transport_header: Transport layer header   传输层相对head的偏移  skb->head + skb->transport_header
 * @network_header: Network layer header       网络层相对head的偏移  skb->head + skb->network_header
 * @mac_header: Link layer header              链路层相对head的偏移  skb->head + skb->mac_header
 * 
 * @tail: Tail pointer                         数据帧的尾部指针, 即payload后一bytes
 * @end: End pointer                           数据帧的终止指针
 * @head: Head of buffer                       数据帧的头指针, 每一层中都一样, 几乎是readonly
 * @data: Data head pointer                    一层协议的数据指针
 *  
 * head | data | tail | end
 */
// "剥掉" 一层协议, len expected 协议头的长度
static inline unsigned char *__skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->len -= len;                  // 下一层协议的数据长度 减少
	BUG_ON(skb->len < skb->data_len);
	return skb->data += len;          // 下一层协议的数据指针 后移
}
// 对于每一层, 刚开始时 skb->data 为这一层数据开始指针
skb->XXX_header = skb->data - skb->head; // 即该层相对于head的位移
// 处理下一层前, 需要"剥掉"该层, 为下一层作准备, 调用__skb_pull(当前层header长度)

skb->data  layer的指针
即 skb->data - sbk->head 得出协议头指针

skb_headlen = skb->len - skb->data_len; // 获取 当层layer的 header长度

#if 0 /* netfilter framework */
input -> netfilter::PRE_ROUTING -> router -> netfilter::FORWARD -->*-> netfilter::POST_ROUTING -> output
                                       v                           ^
.--------------------------------------*                           |
|                                                                  |
*-> netfilter::LOCAL_IN -> inet -> router -> netfilter::LOCAL_OUT -*
#endif