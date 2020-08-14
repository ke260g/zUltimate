  +-------------SW-------------+
  |                            |
  |                            |
 lagX                         lagU
SW(mlag主机)--- peerlink ---Sw(mlag备机)
 lagY                         lagV
  |                            |
  +-------------SW-------------+

双规拓扑中
lagX 与 lagU 的trunk_id 一致
lagY 与 lagV 的trunk_id 一致
正常场景下
1. 来自 lagX/lagY/lagU/lagV 的未知单播, 转发到peerlink.
2. 备机peerlink口收到未知单播,
   不能转发到trunk_id与主机相同, 但是主机和备机都是up的 lag口中 (限制转发)   

+ peerlink口: trunk 口; native vlan无所谓; 但是 allow 所有vlan的报文;
+ peerlink口一旦up, 就开始协商
+ 定时地, 同步 (lagX,lagU) (lagY,lagV) 的mac地址表 (通过 trunk_id 标识)
   且下发到芯片

```
 +-------------SW2-------------+
                               |
                               |
 lagX                         lagU
SW(mlag主机)--- peerlink ---Sw(mlag备机)
 lagY                         lagV
  |                            |
  +-------------SW1------------+
```

断了一个链路: (把lagX的mac同步到 peerlink口; 使得从peerlink口出去)
1. 来自 lagY 的未知单播, 转发到peerlink. (备机 peerlink口学习到mac)
2. 备机peerlink口收到未知单播,
   不能转发到trunk_id与主机相同, 但是主机和备机都是up的 lag口中 (限制转发)   
   + 即备机peerlink口进来的报文, 可以转发到 lagU,其他口  中。

```
  +-------------SW2-------------+
                                |
                                |
 lagX                         lagU
SW(mlag主机)--- peerlink ---Sw(mlag备机)
 lagY                         lagV
                                |
  +-------------SW1-------------+
```
断了两个链路:
1. 主机来自其他口的未知单播, 转发到peerlink. (备机peerlink口学习到mac)
2. 备机peerlink口收到未知单播,
   不能转发到trunk_id与主机相同, 但是主机和备机都是up的 lag口中 (限制转发) 
   + 即备机peerlink口进来的报文, 可以转发到 lagU, lagV, 其他口中。

转发场景:
单芯片:
