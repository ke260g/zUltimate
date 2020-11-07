
__dump_link(struct nlmsghdr *h) {
    struct ifinfomsg *iface;
    struct rtattr *attribute;
    int len;

    iface = NLMSG_DATA(h);
    len = h->nlmsg_len - NLMSG_LENGTH(sizeof(*iface));

    for (attribute = IFLA_RTA(iface); RTA_OK(attribute, len); attribute = RTA_NEXT(attribute, len)) {
        switch(attribute->rta_type) {
        case IFLA_IFNAME:
            printf("link name: %d: %s", iface->ifi_index, (char *) RTA_DATA(attribute));
            break;
        case IFLA_LINKINFO:
            link_info = RTA_DATA(attribute);
            while ((RTA_OK(link_info, len)) && (link_info->rta_type <= IFLA_INFO_MAX)) {
                /* Check for Interface type 'vlan' */
                if ((link_info->rta_type == IFLA_INFO_KIND) && (strcmp(RTA_DATA(link_info), INTERFACE_TYPE_VLAN) == 0)) {
                    printf("vlan name: %d: %s", iface->ifi_index, (char *) RTA_DATA(attribute));
                }
                link_info = RTA_NEXT(link_info, RTA_PAYLOAD(link_info));
            }
            break;
        }
    }
}

dump_link() {
    struct rtattr *rta;
    struct {
        struct nlmsghdr hdr;
        struct rtgenmsg gen;
    } req;

    memset (&req, 0, sizeof(req));

    req.hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
    req.hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.hdr.nlmsg_type = RTM_GETLINK;

    req.gen.rtgen_family = AF_PACKET;

    rta = (struct rtattr *)(((char *)&req) + NLMSG_ALIGN(req.hdr.nlmsg_len));
    rta->rta_len = RTA_LENGTH(4);

    if (send(netlink_sock, &req, req.hdr.nlmsg_len, 0) == -1) {
        assert(0);
    }
    while (!multipart_msg_end) {
        struct sockaddr_nl nladdr;
        struct msghdr msg;
        struct iovec iov;
        struct nlmsghdr *nlh;
        char buffer[RECV_BUFFER_SIZE];
        int ret;

        iov.iov_base = (void *)buffer;
        iov.iov_len = sizeof(buffer);
        msg.msg_name = (void *)&(nladdr);
        msg.msg_namelen = sizeof(nladdr);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        /* In order not to block on the recvmsg, MSG_DONTWAIT
            * is passed as a flag. the return value is EAGAIN if
            * no data is available
            * MSG_DONTWAIT is used during normal flow.
            * On init, we will block till we get updates from
            * the kernel and perform reconfiguration of
            * IP addresses and interfaces
            */
        ret = recvmsg(sock, &msg, on_init ? 0 : MSG_DONTWAIT);

        if (ret < 0) {
            return;
        }

        nlh = (struct nlmsghdr*) buffer;
        for (nlh = (struct nlmsghdr *) buffer; NLMSG_OK(nlh, ret); nlh = NLMSG_NEXT(nlh, ret)) {
            switch(nlh->nlmsg_type) {
                case RTM_NEWLINK:
                    nl_new_link(nlh);
                    break;
            }
        }
    }
}
