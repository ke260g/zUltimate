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
