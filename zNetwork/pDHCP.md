[TOC]
C/S Model
connectionless
UDP: srv-port=67 cli-port=68
![](https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol#/media/File:DHCP_session.svg)

client             server
   | --  discover -> |
   | <-   offer   -- |
   | --  request  -> |
   | <- knowledge -- |

client software sends a DHCP broadcast

DHCP server manages a pool of IP and
  default gateway,
  domain name,
  the name servers,
  time servers

DHCP server may have three methods of allocating IP addresses:
  Dynamic allocation:    无记忆全自动分配, client重发请求会拿到不同IP
  Automatic allocation:  有记忆全自动分配, 记录一个 mac-ip table, client拿到一定时间内永久的IP
  Manual allocation (various):
    static DHCP assignment by DD-WRT
    fixed-address by the dhcpd documentation
    address reservation by Netgear
    DHCP reservation or static DHCP by Cisco and Linksys
    IP address reservation or MAC/IP address binding by various other router manufacturers

DHCP operations fall into four phases: -> (DORA)
  server discovery:         client broadcast or unicast(when it known dhcp server)
    DHCPDISCOVER UDP-Message
    Ethernet: source=sender's MAC; destination=FF:FF:FF:FF:FF:FF
    IP:  source=0.0.0.0;  destination=255.255.255.255
    UDP: source port=68;  destination port=67
  IP lease offer:           server receive DHCPDISCOVER message and unicast to client
    DHCPOFFER message:
    Ethernet: source=server; destination=client
    IP:  source=server;   destination=255.255.255.255 
    UDP: source port=67;  destination port=68
        dhcp content:  the subnet mask, the lease duration 
  IP lease request:         client receive  DHCPOFFER message and broadcast
    DHCPREQUEST message:
    A client can receive DHCP offers from multiple servers, but it will accept only one DHCP offer
    为了给子网内其他处于offer phase的dhcp服务器, 该客户端拿到了IP, 并缓存该IP
  IP lease acknowledgement: server receive DHCPREQUEST and unicast to client
    DHCPACK message:

DHCP options
  in RFC 2132
  defined by Internet Assigned Numbers Authority (IANA)
  octet strings of varying length
    first octet is the option code
    second octet is the number of following octets (该options的参数有多少octets)
    remaining octets are code dependent.           (option code的参数)

Security:
  without any authentication
  3 main types of attacks:
    Unauthorized DHCP servers providing false information to clients
      a fake dhcp server performs a **man-in-the-middle** attack or a **denial-of-service** attack
      fake dhcp server response fake dns server --> DDOS
    Unauthorized clients gaining access to resources
    Resource exhaustion attacks from malicious DHCP clients
See Also:
  BOOTP
