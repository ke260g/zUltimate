# 清除当前规则
iptables -F
iptables -X
iptables -Z

# 设置默认策略 -P 表示 policy
iptables -P INPUT ACCEPT
iptables -P OUTPUT ACCEPT
iptables -P FORWARD ACCEPT

# 重定向目的端口 -j REDIRECT --to-ports
iptables -t nat -A PREROUTING -p tcp --dport 80 -j REDIRECT --to-ports 8080
iptables -t nat -A PREROUTING -p udp --dport 80 -j REDIRECT --to-ports 8080

##########################################################
#                                                        #
# client... <-> proxy 192.168.1.1 <-> real 192.168.1.2   #
#                                                        #
##########################################################
#
# 在 proxy 进行如下重定向 [1] + { [2] or [3] }
# [1] 重定向目的地址 -j DNAT --to-destination
iptables -t nat -A PREROUTING -d 192.168.1.1 -p tcp -m tcp --dport 80 -j DNAT --to-destination 192.168.1.2
# [2] 重定向源  地址 -j SNAT --to-source       (如此一来 real 只认识 proxy )
iptables -t nat -A POSTROUTING -d 192.168.1.2 -p tcp -m tcp --dport 80 -j SNAT --to-source 192.168.1.1
# [3] 透明转发                                 (如此一来 使得 real 感知实际的 client)
iptables -t nat -A POSTROUTING -d 192.168.1.2 -p tcp --dport 80 -j MASQUERADE