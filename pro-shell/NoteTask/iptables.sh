#!/sbin/openrc-run
IPT=iptables
ETH="eth0"
WAN="wlan0"

SOCKS_PORT="1080" # socks4, socks5
# OPENWINDOWS_PORT="2000"
SQUID_PORT="3128" # squid
NFS_PORT="2049"
NFS_RPC_LOCKD_PORT="4045"

NON_PRI_PORTS="$NFS_PORT,$SOCKS_PORT,$SQUID_PORT"

depend () {
    need net
} 

start () {
    ebegin "start firewall"
    # allow loop
    $IPT -A INPUT -i lo -j ACCEPT
    $IPT -A OUTPUT -o lo -j ACCEPT

    # broadcast issue

    # well know port
    $IPT -A OUTPUT -p udp --dport 53 -j ACCEPT # dns client
    $IPT -A OUTPUT -p tcp --dport 53 -m state --state NEW -j ACCEPT

    $IPT -A OUTPUT -p udp --dport 67 -j ACCEPT # bootps, dhcp server
    $IPT -A OUTPUT -p udp --dport 68 -j ACCEPT # bootpc, dhcp client

    #$IPT -A OUTPUT -p tcp --dport 123 -j ACCEPT # ntp/tcp
    $IPT -A OUTPUT -p udp --dport 123 -j ACCEPT # ntp/udp

    $IPT -A INPUT -p tcp --sport 80 -j ACCEPT # http/tcp
    $IPT -A OUTPUT -p tcp --dport 80 -j ACCEPT # http/tcp
    $IPT -A INPUT -p tcp --sport 443 -j ACCEPT # https/tcp
    $IPT -A OUTPUT -p tcp --dport 443 -j ACCEPT # https/tcp

    $IPT -A INPUT -p tcp --sport 22 -j ACCEPT # avoid ssh login
    $IPT -A INPUT -p udp --sport 22 -j ACCEPT # avoid ssh login
    $IPT -A OUTPUT -p tcp --dport 22 -j ACCEPT # ssh

    $IPT -A OUTPUT -p tcp --dport 993 -j ACCEPT # imaps/tcp
    $IPT -A OUTPUT -p tcp --dport 995 -j ACCEPT # pop3s/tcp
    $IPT -A OUTPUT -p tcp --dport 465 -j ACCEPT # smtps/tcp
    $IPT -A OUTPUT -p udp --dport 993 -j ACCEPT # imaps/udp
    $IPT -A OUTPUT -p udp --dport 995 -j ACCEPT # pop3s/udp
    $IPT -A OUTPUT -p udp --dport 465 -j ACCEPT # smtps/dcp

#    $IPT -A INPUT -p tcp --dport 20 -j ACCEPT # ftp/tcp, data
#    $IPT -A OUTPUT -p tcp --dport 20 -j ACCEPT # ftp/tcp, data,

    $IPT -A INPUT -p tcp --dport 21 -j ACCEPT # ftp/tcp, control, listen
    $IPT -A INPUT -p tcp --dport 20921 -j ACCEPT # ftp/tcp, control, listen

    $IPT -A OUTPUT -p tcp --dport 21 -j ACCEPT # ftp/tcp, control, access out
    $IPT -A OUTPUT -p udp --dport 21 -j ACCEPT # ftp/udp, control, access out

    # icmp
    $IPT -A INPUT -p icmp -j ACCEPT
    $IPT -A OUTPUT -p icmp -j ACCEPT

    # handle connection-state
    $IPT -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
    $IPT -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

    $IPT -A INPUT -m state --state INVALID -j LOG\
        --log-prefix "INVALID input: "
    $IPT -A INPUT -m state --state INVALID -j DROP

    $IPT -A OUTPUT -m state --state INVALID -j LOG\
        --log-prefix "INVALID output: "
    $IPT -A OUTPUT -m state --state INVALID -j DROP

    # at last, DROP all others
    $IPT -P INPUT DROP
    $IPT -P FORWARD DROP
    $IPT -P OUTPUT DROP
}

stop () {
    ebegin "stop firewall"
    $IPT -F INPUT
    $IPT -F FORWARD
    $IPT -F OUTPUT

    $IPT -P INPUT ACCEPT
    $IPT -P FORWARD ACCEPT
    $IPT -P OUTPUT ACCEPT
}
