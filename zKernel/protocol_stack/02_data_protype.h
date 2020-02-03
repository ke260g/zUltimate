struct msghdr {
	struct iovec	*msg_iov;	/* scatter/gather array */
};

struct iovec {
	__kernel_size_t iov_len; /* Must be size_t (1003.1g) */
};

struct file_operations {

};

struct file {
	const struct file_operations	*f_op;
} __attribute__((aligned(4)));	/* lest something weird decides that 2 is OK */

// include/linux/net.h
struct socket {
	struct file		*file;
	struct sock		*sk;
	const struct proto_ops	*ops;
};

// include/net/sock.h
struct sock {
	struct sock_common	__sk_common;
	struct socket		*sk_socket;
};

// include\net\inet_sock.h
struct inet_sock {
    /* sk and pinet6 has to be the first two members of inet_sock */
	struct sock		sk;
#if IS_ENABLED(CONFIG_IPV6)
	struct ipv6_pinfo	*pinet6;
#endif
};

struct udp_sock {
	/* inet_sock has to be the first member */
	struct inet_sock inet;
};

struct inet_connection_sock {
	/* inet_sock has to be the first member! */
	struct inet_sock	  icsk_inet;
};

struct tcp_sock {
	/* inet_connection_sock has to be the first member of tcp_sock */
	struct inet_connection_sock	inet_conn;
};

// 每个CPU都有 入列缓存
struct softnet_data { };
DECLARE_PER_CPU_ALIGNED(struct softnet_data, softnet_data);

// 原始 网络帧  skbuff.h
struct sk_buff {
    /* These two members must be first. */
    struct sk_buff      *next;
    struct sk_buff      *prev;
    struct sock      *sk;
};

struct sk_buff_head {
    /* These two members must be first.*/
    struct sk_buff    *next;
    struct sk_buff    *prev;
};

// 网络设备对象
struct net_device { };

struct proto_ops { }; // socket操作
struct proto { }; // sock操作


/* This is used to register socket interfaces for IP protocols.  */
struct inet_protosw {
    unsigned short    type;    /* This is the 2nd argument to socket(2). */
    unsigned short    protocol; /* This is the 3rd argument to socket(2) */
    struct proto *prot;
    const struct proto_ops *ops;
};

/* This is used to register protocols. */
struct net_protocol {
	int			(*early_demux)(struct sk_buff *skb);    // 紧接 NF_INET_PRE_ROUTING 后
	int			(*handler)(struct sk_buff *skb);        // 紧接 NF_INET_LOCAL_IN    后
	void		(*err_handler)(struct sk_buff *skb, u32 info);
};

/* This is used to register ether_frame */
struct packet_type {
    __be16			type;	/* This is really htons(ether_type). */
    struct net_device	*dev;	/* NULL is wildcarded here	     */
    int			(*func) (struct sk_buff *,
					 struct net_device *,
					 struct packet_type *,
					 struct net_device *);
	bool			(*id_match)(struct packet_type *ptype,
					    struct sock *sk);
};
