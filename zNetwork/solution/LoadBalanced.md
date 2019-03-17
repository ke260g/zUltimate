网络负载均衡问题:
三层 DNS LVS APP
DNS 出个 domain对应n个ip
APP 应用层服务器, 如nginx进行正/反向代理, redis/kafka 服务订阅
LVS Linux Virtual Server, Layer4 LoadBalancing

### LVS 有三个实现方式  
+ NAT              性能瓶颈较大, 吞吐量完全依赖虚拟服务器, 且需要在一个子网段
	+ 形同路由器的工作方式, (内网穿透)
	+ 通过更多的IP端口 识别 实体服务器
	+ 响应包 需要经过 虚拟服务器
+ Ip Tunnel        性能较高, 虚拟服务器与实体服务器 可以 组成三层网络
	+ 本质上是多一次Ip封包和解包协议
	+ 响应包 直接发给 客户端
+ Direct Routing   性能最高, 但是必须在同一个二层交换机中, 或路由器中
	+ 链路层转发, 不经过IP层
	+ 本质是 实体服务器 不丢弃 DEST-IP为虚拟服务器IP的包, 注意DEST-IP不同于实体服务器IP
	+ 响应包 直接发给 服务端

### LVS 已有方案
ipvsadm, 				(layer-4 switching)
ktcpvs, tcpvsadm		(layer-7 switching)
tcpsp, (tcp splicing)   (layer-7 switching)
tcpha, (tcp handle of)  (layer-7 switching)
Keepalived, VRRP协议
Heartbeat, Corosync
