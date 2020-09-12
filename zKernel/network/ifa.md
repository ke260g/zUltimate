内核中的 ip地址表征

ifconfig eth0:sub_interfacee 4.4.4.4 netmask 255.0.0.0

net/ipv4/devinet.c::devinet_ioctl
case SIOCSIFADDR  对 ":" 进行了额外处理

1. ":" 前面的字符串作为原始的 net_device 索引
2. 新建一个 in_ifaddr ifa 对象;
   掩码 地址 都设置到这个 ifa对象里面
3. inet_set_ifa(dev, ifa); 
   1. 设置 ifa->ifa_dev = in_dev;
   2. 调用 inet_insert_ifa(ifa); 把 ifa对象插入到全局系统 和 dev 的地址列表中
    inet_hash_insert(dev_net(in_dev->dev), ifa); // 全局系统
    (struct net_device *)dev -> (struct in_device *)ip_ptr -> (struct in_ifaddr *)ifa_list // 网卡设备的地址列表
4. ifconfig 获取网卡设备时; 使用SIOCGIFCONF 指令;
    1. 该指令 遍历 net_device 时; 顺带着返回 net_device 的 ifa_list 每个ip地址

```c++
net\core\dev_ioctl.c::dev_ifconf
for_each_netdev(net, dev) {
    for (i = 0; i < NPROTO; i++) {
        if (gifconf_list[i]) {
            int done;
            if (!pos)
                done = gifconf_list[i](dev, NULL, 0);
            else
                done = gifconf_list[i](dev, pos + total,
                                len - total);
            if (done < 0)
                return -EFAULT;
            total += done;
        }
    }
}

static gifconf_func_t *gifconf_list[NPROTO]; // 定义; 函数回调表
int register_gifconf(unsigned int family, gifconf_func_t *gifconf); // 注册
register_gifconf(PF_INET, inet_gifconf); // net\ipv4\devinet.c::devinet_init 中注册

1. net_namespace 里面有多个 net_device
2. gifconf_list[i] 有多个proto表
3. gifconf_list[i] 每个表  对应于每个 net_device 有表项
4. gifconf_list[AF_INET] 即 inet_gifconf
5. inet_gitconf 遍历返回 dev->in_dev->ifa_list 的每个对象(ip 地址)
6. (struct net_device *)dev -> (struct in_device *)ip_ptr -> (struct in_ifaddr *)ifa_list 地址列表
```