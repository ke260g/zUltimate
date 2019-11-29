#!/bin/sh
# telnet 需要连接后 再输入, 没连接的话, 键盘输入不会被缓存
# 所以需要 sleep 1 && cmd && sleep 2 && cmd | telnet $ip $port
MOD=61 # 
SEC=60 # per minitues
ip=127.0.0.1
port= 
while true; do
	filename=/var/module_$(date +%Y%m%d_%X).telnet.log
	((sleep 1 && echo "dlanrecover" && sleep 2 && echo "set $MOD specific cmd" && sleep $SEC) | telnet $ip $port) > $filename
done