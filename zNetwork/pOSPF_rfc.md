[TOC]
# reading memo of rfc2328
一个AS中用同一个IGP,
OSPF是一种IGP, 另一种IGP是RIP
一个AS中有多个Area,
一个Router可同时从属多个Area,
即Area中间是用Router相连的

Designated Router: generated LSA, 减少每个Router都要回LSA的浪费

link-state database 描述了一个直观的图
图的矢量 由 routers and networks 组成
图的边   两个物理上直连的router
        routers由interface在network中  <-- 这个network是不是area的意思？

1. ospf 是 IGP, 同类的协议有 RIP, IGP对应是 EGP
2. IGP用于同一个AS中, 一个AS内部所有Router都实现同一个IGP,(这里用的是ospf)
   后面只讨论实现ospf的AS
3. 一个AS 可以生成多个 area (多数以网段区分)
4. ospf 有且只有5种类型包: 
   Hello
   Database Description(DBD)
   Link State Request(LSR)
   Link State Update(LSU)
   Link State Acknowledgment(LSAck)
5. TD描述中的 lsa
   1. LSU中有n个LSA
   2. DBD中有1个LSA header

`Hello`定时地发
收到`Hello`有可能根据里面的内容建立adjacency
通过`Hello`初始化adjacency后发`DBD`
收到`DBD`,发现自己的LinkStateDatabase需要更新, 发送`LSR`请求更新,
收到`LSR`, 则发送`LSU`,`LSU`里面有LSAs
收到`LSU`, 更新自己的数据库, 然后广播

### 5 types LSA
Router-LSAs
Each router in an area originates a router-LSA
Network-LSAs originated for
    each broadcast and NBMA network in the area which supports two or
    more routers.  The network-LSA is originated by the network's
    Designated Router. The LSA describes all routers attached to the
    network, including the Designated Router itself.
Summary-LSAs (IP network)
Summary-LSAs (ASBR)
AS-external-LSAs

 Every router
    originates a router-LS

我看到说  每个 area有自己的link-state-database。
之前看到  每个as有全局同步的link-state-database。

这个是那个lsa,普通的有5类，有的是区域内同步，有的是全局同步