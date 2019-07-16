[TOC]
OSPF (open shortest path first)
dynamic routing protocol
interior gateway protocol

# Flow
1. hello packets are sent out of each participating non-passive interface
2. These packets form neighborships and adjacencies with other routers on the same subnet
3. an adjacency has the capability of sending routing information
4. a neighborship only sends link-state information used to build a map of links within the network

## Adjacencies
Adjacencies are formed between routers that have specific **roles** within the OSPF topology.
### There are three roles within an OSPF topology
+ the designated router (DR)
+ the backup designated router (BDR)
+ designated other (DROTHER). 
There is only one DR and one BDR, but there can be any number of DROTHERs. Each DROTHER has two adjacencies. One is between the DR and the other is between the BDR. 

The DR and BDR is determined through an election process during the start up of OSPF.
The router with the lowest router-id is elected DR, the second highest BDR.
Since this election occurs during startup, the router with the highest router-id may be elected as the DR, because the other routers within the OSPF topology ran the election process after the DR and BDR had already been determined.
A router only goes through the election process at the initial startup of OSPF. 

OSPF is always organized in Areas. An area is a network where Link-State-Advertisements (LSA) are distributed to each router and is identified by a 4-byte integer, some times displayed like an IPv4-address (Pitfall!)


[TOC]
# OSPF
Open Shortest Path First
OSPF: LSR-based protocol, is a widely used IGP in large enterprise networks
ISIS: LSR-based protocol, is more common in large service provider networks.
理论上存在: 单体工作状态机 以及 多段协商流程图
## High Perspective Functionality
link state routing (LSR) algorithm
falls into the group of interior gateway protocols (IGPs)
operating within a single autonomous system (AS) (such as a local area network (LAN))
## Core Working Feature
link state databases on every router 
  + build real network topology maps
Dijkstra's algorithm (SPF) algorithm
A router interface with OSPF will then advertise its link cost to neighbouring routers through multicast
keep sending hello packets, and thus changes in the cost of their links become known to neighbouring routers.

The information about the cost of a link,  is then cascaded through the network because OSPF routers advertise the information they receive from one neighbouring router to all other neighbouring routers.

1. 维护 link-state-database
2. 用Dijkstra's algorithm求最短路径
3. 不断发送HelloPackets, 通知附近路由器 link-cost
4. cascade 整个网络 当收到邻居的 link-cost信息
5. cascade 过程称为"同步"
6. 收到 邻居的link-cost更新 自己的 link-state-database表

## relationship
the relationship between two routers is adjacencies or neighbors.
neighbor relationship table (adjacency database)
Two routers must first be neighbors and only then, can they become adjacent.
an interface has configureed a **OSPF Area**
### neighbor
#### form condition
1. in the same subnet and share the same area ID, subnet mask, timers and authentication.
2. at each end of a point-to-point link communicate with each other via the OSPF protocol.
#### functionality
neighbors do not exchange any routing information
neighbors exchange only Hello packets.
In essence, OSPF neighborship is a relationship between two routers that allow them to see and understand each other but nothing more.
### adjacency
#### form condition
1. routers have **detected** each other
2. This detection is initiated when a router identifies itself in a Hello protocol packet
3. Two neighbor allow them each other to exchange routing information
4. Two routers become adjacent if
    1. at least one of them is esignated Router or 
    2. Backup Designated Router (on multiaccess type networks), or
    3. they are interconnected by a point-to-point or point-to-multipoint network type. 
#### functionality

## state machine
Down > Attempt > Init > **2-Way** > ExStart > Exchange > Loading > Full

## message
### Hello
1. routers send hello messages to their neighbors at regular intervals (the hello interval)
2. if a router stops receiving hello messages from a neighbor, 
   after a set period (the dead interval) the router will assume the neighbor has gone down.
### Database Description (DBD)
LSU 的 内容, 用于同步数据库
### Link State Request (LSR)
主动发起
### Link State Update (LSU)
主动组播, 或作为LSR的响应, 然后通过DBD同步数据库
### Link State Acknowledgment (LSAck)
LSU的ack
DBD的ack

## OSPF area
An OSPF network can be divided into areas that are logical groupings of hosts and networks.
Areas are uniquely identified with 32-bit numbers.
Each area maintains a separate link state database.
For a single router can be in multi area at the same time.
The topology of an area is unknown outside the area.
### Backbone 
### Stub area
### Not-so-stubby area
### Proprietary extensions
Several vendors (Cisco, Allied Telesis, Juniper, Alcatel-Lucent, Huawei, Quagga)
implement the two extensions below for stub and not-so-stubby areas
### Transit area

## Router types
Internal router (IR)
Area border router (ABR)
Backbone router (BR)
Autonomous system boundary router (ASBR)


rfc 描述要比 wiki清晰很多
what is Interior Gateway Protocol (IGP)
what is Exterior Gateway Protocol (BGP)

Interior Gateway Protocol
    The routing protocol spoken by the routers belonging to an
    Autonomous system.  Abbreviated as IGP.  Each Autonomous
    System has a single IGP.  Separate Autonomous Systems may be
    running different IGPs.
