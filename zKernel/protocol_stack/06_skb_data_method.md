该文章讨论 skb 关键的成员, 
1. 操作方法
2. 底层实现
3. 例子分析


对象定义 
```c++
struct sk_buff {
    unsigned int len, data_len;
    __u16 mac_len, hdr_len;
	/* These elements must be at the end, see alloc_skb() for details.  */
	sk_buff_data_t		tail;
	sk_buff_data_t		end;
	unsigned char		*head,
                        *data;
};

void kfree_skb(struct sk_buff *skb); // 销毁
struct sk_buff *skb_clone(struct sk_buff *skb, gfp_t priority);
struct sk_buff *skb_copy(const struct sk_buff *skb, gfp_t priority);

struct sk_buff *dev_alloc_skb(unsigned int length);
```
# 关键的成员 
1. head
2. data
3. tail, 
4. end, 
5. len,
6. data_len
+ 猜想: 完整的data_len, 该layer的len, 完整的data指针, 该layer的指针, 完整的data结尾, 该layer的数据结尾

```c++
typedef unsigned int sk_buff_data_t; // 使用偏移法时, 该type是 int; 使用指针法, 该type是指针

skb_end_pointer = skb->head + skb->end;
skb_end_offset = skb->end;
skb_headlen = skb->len - skb->data_len; ??
skb_tail_pointer = skb->head + skb->tail;
skb_headroom = skb->data - skb->head;
skb_tailroom = skb->end - skb->tail;
skb_availroom = skb->end - skb->tail - skb->reserved_tailroom;
sbk_reserved_tailroom

mac_header_len = skb->network_header - skb->mac_header;

// 偏移 方法
// This function extends the used data area of the buffer.
skb_put(sbk, len)  { skb->tail += len; skb->len  += len; }
// This function extends the used data area of the buffer at the buffer start.
skb_push(skb, len) { skb->data -= len; skb->len  += len; } 
// This function removes data from the start of a buffer.
skb_pull(skb, len) { skb->len -= len; skb->data += len; } 
// Increase the headroom of an empty &sk_buff by reducing the tail room.
skb_reserve(skb, len) { skb->data += len; skb->tail += len; }

// reset 方法,
skb_reset_mac_len(skb) { skb->mac_len = skb->network_header - skb->mac_header; }
skb_reset_mac_header(skb) {	skb->mac_header = skb->data - skb->head; }
skb_reset_network_header(skb) {	skb->network_header = skb->data - skb->head; }
skb_reset_transport_header(skb) { skb->transport_header = skb->data - skb->head; }
skb_reset_tail_pointer(skb) { skb->tail = skb->data - skb->head; }

// skb 中拿数据
skb_copy_from_linear_data(const struct sk_buff *skb,void *to, const unsigned int len) 
{ memcpy(to, skb->data, len); }
// 拷数据到 skb
skb_copy_to_linear_data(struct sk_buff *skb,const void *from, const unsigned int len)
{	memcpy(skb->data, from, len); }


write: reserve + (push + copy_to)*N
read:  (pull + copy_from)*N
```

1. head 完整报文头指针
2. data 该层报文头指针
3. len  该层报文的长度
4. tail 该层报文的尾部??
5. end  完整报文的结尾??


1. 以外网帧的头, 是哪里填充的, 驱动吗?
2. 如果我用户态发送 二层报文, 里面带上了以太网帧的头, 如何通知驱动不要再加一次?


brctl show
cat /sys/class/net/eth0/ifindex

# 队列操作
// 创建
void skb_queue_head_init(struct sk_buff_head *list);
// 出列 模板1
struct sk_buff *skb_peek(const struct sk_buff_head *list_);      // 非原子操作; 需要外层锁
struct sk_buff *skb_peek_tail(const struct sk_buff_head *list_); // 非原子操作; 需要外层锁
void skb_unlink(struct sk_buff *skb, struct sk_buff_head *list); // 出列后手动释放
// 出列 模板2
struct sk_buff *skb_dequeue(struct sk_buff_head *list);
struct sk_buff *skb_dequeue_tail(struct sk_buff_head *list);
// 入列
void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);

# 销毁方法
```c++
void dev_kfree_skb(struct sk_buff *skb); // 非中断上下文中
void dev_kfree_skb_irq(struct sk_buff *skb); // 中断上下文中
void dev_kfree_skb_any(struct sk_buff *skb); // 内嵌判断逻辑 自适应free; 耗性能


```