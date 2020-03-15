SYSCALL_DEFINE3(sendmsg, int, fd, struct msghdr __user *, msg, unsigned int, flags)
{
    // ...
	return __sys_sendmsg(fd, msg, flags);
}

long __sys_sendmsg(int fd, struct msghdr __user *msg, unsigned flags)
{
    // ...
    // 根据fd索引socket
	sock = sockfd_lookup_light(fd, &err, &fput_needed);
	if (!sock)
		goto out;
    // 发包
	err = ___sys_sendmsg(sock, msg, &msg_sys, flags, NULL, NULL);
    // 销毁file对象
	fput_light(sock->file, fput_needed);
out:
	return err;
}
// 该函数 本质把 msghdr 从用户态, 切到内核态
// msghdr 中的成员有 sockaddr_storage 和 iovec[]
// 该函数 把msghdr的成员 iovec[] 和 sockaddr_storage 切到内核态
// iovec->iov_base才是物理内存意义上的 userspace,
// 后续收包逻辑, 才把sk_buff数据 复制到 iovec->iov_base中
static int ___sys_sendmsg(struct socket *sock, struct msghdr __user *msg,
			 struct msghdr *msg_sys, unsigned int flags,
			 struct used_address *used_address, int *residue)
{
    // msg_sys kernelspace, 用于临时存储userspace的msg
	struct compat_msghdr __user *msg_compat =
	    (struct compat_msghdr __user *)msg;
	struct sockaddr_storage address;
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;
	unsigned char ctl[sizeof(struct cmsghdr) + 20]
	    __attribute__ ((aligned(sizeof(__kernel_size_t))));
	/* 20 is size of ipv6_pktinfo */
	unsigned char *ctl_buf = ctl;
	int err, ctl_len, total_len;

	err = -EFAULT;

    // userspace 的 msg 赋值到 msg_sys
	if (MSG_CMSG_COMPAT & flags)
		err = get_compat_msghdr(msg_sys, msg_compat);
	else
		err = copy_msghdr_from_user(msg_sys, msg);
	if (err)
		return err;

    // 分配足够多内核态 iovec
    // iovstack[UIO_FASTIOV] 默认放在栈上的 若干个
	if (msg_sys->msg_iovlen > UIO_FASTIOV) {
		err = -EMSGSIZE;
		if (msg_sys->msg_iovlen > UIO_MAXIOV)
			goto out;
		err = -ENOMEM;
		iov = kmalloc(msg_sys->msg_iovlen * sizeof(struct iovec),
			      GFP_KERNEL);
		if (!iov)
			goto out;
	}

	/* This will also move the address data into kernel space */
    // verify_iovec 本质; userspace 切换到 kernelspace
    // 1. msg_sys->msg_name = addr; 指向kernelspace       => move_addr_to_user
    // 2. msg_sys->msg_iov指向的数据 赋值到 iov指向的数据 => copy_from_user, msg_sys->msg_iov之前是指向userspace的
    // 3. msg_sys->msg_iov = iov
    // Note: msg_iov->iov_base 才是指向物理意义上的用户空间, 用户提供的数据
	if (MSG_CMSG_COMPAT & flags) {
		err = verify_compat_iovec(msg_sys, iov, &address, VERIFY_READ);
	} else
		err = verify_iovec(msg_sys, iov, &address, VERIFY_READ);
	if (err < 0)
		goto out_freeiov;
	total_len = err;

	err = -ENOBUFS;

    // ctl 控制参数, 用户态复制到内核态
	msg_sys->msg_flags = flags;

	if (sock->file->f_flags & O_NONBLOCK)
		msg_sys->msg_flags |= MSG_DONTWAIT;
	/*
	 * If this is sendmmsg() and current destination address is same as
	 * previously succeeded address, omit asking LSM's decision.
	 * used_address->name_len is initialized to UINT_MAX so that the first
	 * destination address never matches.
	 */
	if (used_address && msg_sys->msg_name &&
	    used_address->name_len == msg_sys->msg_namelen &&
	    !memcmp(&used_address->name, msg_sys->msg_name,
		    used_address->name_len)) {
		err = sock_sendmsg_nosec(sock, msg_sys, total_len);
		goto out_freectl;
	}
	err = sock_sendmsg(sock, msg_sys, total_len);
    // ...
    // 切回用户态
	if (used_address && err >= 0) {
		used_address->name_len = msg_sys->msg_namelen;
		if (msg_sys->msg_name)
			memcpy(&used_address->name, msg_sys->msg_name,
			       used_address->name_len);
	}
    // ...
	return err;
}