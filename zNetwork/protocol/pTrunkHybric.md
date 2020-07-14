[TOC]
# Trunk and Hybric
二层口才分: access and trunk and hybric
三层口只有: access
三层口在转发的场景中, 相当于一个网关
同一个交换机中: 每个Vlan的网段, 每个三层口的网段 都不能重复
## similar concept
每个端口 至少存在4个本质属性
1. 哪些报文从中进来
2. 进来的报文, 进来前做哪些预操作
3. 哪些**未知单播**报文从中出去
4. 出去的报文, 出去前做哪些预操作
### Access
Native_Vlan:
1. 任何报文可以进来
4. 带有 VLAN_ID 未知单播, 可以通过(Native_Vlan == VLAN_ID)的Access口广播出去
   没有 VLAN_ID 未知单播, 可以从与sip同一网段的Access口广播出去
### Trunk
宏观抽象: 把多个VLAN 绑在同一个物理口
Native_Vlan
Vlan_Group: VLAN_ID集合
Note: Native_Vlan 可以不在Vlan_Group其中
1. VLAN_ID == Native_Vlan的报文才能进来
4. 带有 VLAN_ID 未知单播, 从**包含该VLAN_ID的Vlan_Group**的端口出去 (仅影响输出报文)
   没有 VLAN_ID 未知单播, 插入VLAN_ID后从Trunk口出去
#### Hybric 升级版Trunk
Native_Vlan:   Untagged Vlan和Tagged Vlan的并集中的一个元素
Untagged_Vlan: (仅影响输出报文), 类似Vlan_Group, 但输出报文不打上VLAN_ID
Tagged_Vlan:   (仅影响输出报文), 类似Vlan_Group, 但输出报文
Untagged_Vlan 和 Tagged_Vlan 没有交集
1. 任何报文都能进来
2. 进来的报文如果没有 VLAN_ID, 则插入 Native_Vlan
3. VLAN_ID 属于 Untagged_Vlan 或 Tagged_Vlan 的未知单播可以广播出去
4. 如果属于 Untagged_Vlan 则去掉VLAN_ID 再广播
   如果属于 Tagged_Vlan   则直接广播
## Flow
### 一个带有 VLAN_ID 的报文 进来
### 一个没有 VLAN_ID 的报文 进来

