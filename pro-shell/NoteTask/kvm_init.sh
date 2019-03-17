#!/bin/sh
#
# network
tunctl -t tap0 -u ll
ifconfig tap0 up
brctl addbr br0 
brctl addif br0 enp5s0
brctl addif br0 tap0
ifconfig br0 up

echo 1 > /proc/sys/net/ipv4/ip_forward
iptables -t nat -A POSTROUTING -j MASQUERADE
