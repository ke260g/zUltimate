SYSCALL_DEFINE5(recvmmsg, int, fd, struct mmsghdr __user *, mmsg,
		unsigned int, vlen, unsigned int, flags,
		struct timespec __user *, timeout)
{
	if (!timeout) // 无 timeout, 非阻塞, 5th-argument 为 NULL
		return __sys_recvmmsg(fd, mmsg, vlen, flags, NULL);
    // ... copy_from_user timeout参数
	datagrams = __sys_recvmmsg(fd, mmsg, vlen, flags, &timeout_sys);
    // ... cooy_to_user   timeout参数(返回输出)
	return datagrams;
}

int __sys_recvmmsg(int fd, struct mmsghdr __user *mmsg, unsigned int vlen,
		   unsigned int flags, struct timespec *timeout)
{
    // ...
    // 通过fd 获取 socket
    // 1. int fd 获取 struct fd
    // 2. fd->file->private_data, sock_map_fd()函数中设置private_data为socket
 	datagrams = 0;
	sock = sockfd_lookup_light(fd, &err, &fput_needed);
    // ...

	entry = mmsg;
	compat_entry = (struct compat_mmsghdr __user *)mmsg;

	while (datagrams < vlen) {
		if (MSG_CMSG_COMPAT & flags) {
            // ... 处理 MSG_CMSG_COMPAT 逻辑, 
            // ... 仅 entry 与 compat_entry 的结构体稍微不同
		} else {
			err = ___sys_recvmsg(sock,
					     (struct msghdr __user *)entry,
					     &msg_sys, flags & ~MSG_WAITFORONE,
					     datagrams);
			if (err < 0)
				break;
			err = put_user(err, &entry->msg_len);
			++entry;
		}

		if (err)
			break;
		++datagrams;

		/* MSG_WAITFORONE turns on MSG_DONTWAIT after one packet */
		if (flags & MSG_WAITFORONE)
			flags |= MSG_DONTWAIT;

		if (timeout) {
            // ... 超时break逻辑
		}

		/* Out of band data, return right away */
		if (msg_sys.msg_flags & MSG_OOB)
			break;
	}
    // ... handler errror
out_put:
    // if sockfd_lookup_light 找不到 socket
    // 则 fput_light 销毁 file
	fput_light(sock->file, fput_needed);

	return datagrams;
}

// 该函数 本质把 msghdr 从用户态, 切到内核态
// msghdr 中的成员有 sockaddr_storage 和 iovec[]
// 该函数 把msghdr的成员 iovec[] 和 sockaddr_storage 切到内核态
// iovec->iov_base才是物理内存意义上的 userspace,
// 后续收包逻辑, 才把sk_buff数据 复制到 iovec->iov_base中
static int ___sys_recvmsg(struct socket *sock, struct msghdr __user *msg,
			 struct msghdr *msg_sys, unsigned int flags, int nosec)
{
    // msg_sys kernelspace, 用于临时存储userspace的msg
	struct compat_msghdr __user *msg_compat =
	    (struct compat_msghdr __user *)msg;
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;
	unsigned long cmsg_ptr;
	int err, total_len, len;

	/* kernel mode address */
	struct sockaddr_storage addr;

	/* user mode address pointers */
	struct sockaddr __user *uaddr;
	int __user *uaddr_len;

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

	/* Save the user-mode address (verify_iovec will change the
	 * kernel msghdr to use the kernel address space)
	 */
	uaddr = (__force void __user *)msg_sys->msg_name;
	uaddr_len = COMPAT_NAMELEN(msg);
    // verify_iovec 本质; userspace 切换到 kernelspace
    // 1. msg_sys->msg_name = addr; 指向kernelspace       => move_addr_to_user
    // 2. msg_sys->msg_iov指向的数据 赋值到 iov指向的数据 => copy_from_user, msg_sys->msg_iov之前是指向userspace的
    // 3. msg_sys->msg_iov = iov
	if (MSG_CMSG_COMPAT & flags)
		err = verify_compat_iovec(msg_sys, iov, &addr, VERIFY_WRITE);
	else
		err = verify_iovec(msg_sys, iov, &addr, VERIFY_WRITE);
	if (err < 0)
		goto out_freeiov;

    // ...
    // 收包
	err = (nosec ? sock_recvmsg_nosec : sock_recvmsg)(sock, msg_sys,
							  total_len, flags);
    // ...
    // 切回用户态
	if (uaddr != NULL) {
		err = move_addr_to_user(&addr,
					msg_sys->msg_namelen, uaddr,
					uaddr_len);
		if (err < 0)
			goto out_freeiov;
	}
    // ...
out:
	return err;
}