[TOC]
LAG is defined in the IEEE 802.1AX-2008 standard, where MC-LAG is not involved.
MC-LAG networking is introduced by Arista in 2012.
MC-LAG networking is introduced by Arista in 2012.MLAG implementations are vendor-specific.
Say MC-LAG Juniper and mLACP Cisco. 
However, the combined chassis is still compliant to the IEEE 802.1AX-2008 standard.

Rooted in LAG but not ceased to advance
LAG 包括但不限于

## evolution history
![](https://upload.wikimedia.org/wikipedia/commons/2/2f/Using_Multi-Chassis_LAG_%28MC-LAG%29_for_High_Availability.png)




### Access and Trunk and Hybrid
Access: 入口报文如果vlan
Native_Vlan: 只接受带Vlan_Id的报文

### LAG
宏观抽象: 把多个物理口绑定成同一个物理口
多端口的LoadBalance方案 有两种
#### 手工协商
通过"属性"生成hash_code, 然后取模分配到不同的直连链路中
"属性"是人为定义还是标准定好?
dip
dmac
sip
smac
sip  and dip
smac and dmac
#### 动态协商
LACP (Link Aggregation Control Protocol)
接口LACP优先级值越小，优先级越高。
主动模式：能够发送和接收LACP协议报文
被动模式：只能接收LACP协议报文
### MLAG
宏观抽象: 把两台设备, 绑定成同一台
```
 +----- WAN -------+
 |                 |
M1                 M2
 |                 |
 +--  keep alive --+  <-- keep alive必须是三层口
A1 -- peer link -- A2 <-- peer link是二层口
|||               ||| <-- 这里配了 MLAG
 *                 *
 *                 *
|||               ||| <-- 这里配了 LAG
 +------ B --------+
```
A1 and A2配了MLAG, 对于A1 or A2:
1. 从peer-link进来的包，广播/组播/未知单播
   不会转到MLAG口出去
2. 对于B来说，不需要知道 A1,A2是否配了MLAG
3. 如果从MLAG口 进来的包的 DMAC 要转发的口 挂掉了，
   那就从 peer-link中转出去
4. 从peer-link进来的包, 已知单播,
   允许转到MLAG口出去,
   这功能需要A1和A2协商的
5. 如果M1挂掉
  + 前期上行报文 B -> A1 -> A2 -> M2 -> WAN
  + 下行报文 WAN -> M2 -> A2 -> B
  + 后期上行报文 B -> A2 -> M2 -> WAN (B中的MAC地址漂移了)
Note: 物理世界中这只是短期的, 人类活动会尽快修好M1
6. keep alive 并不是必要的, 不过现实中非常有用
   逻辑抽象: 用于识别对端机子
   实际用途: 当MLAG出故障时, 通过该端口diagnosis
7. 理论上是可以N台组成MLAG的