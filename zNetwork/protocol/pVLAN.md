|port-type|property|in-direction|out-direction|
|:-:|:-:|:-:|:-:|:-:|
|access|NativeVlan|无VLAN的报文,打上VLAN|剥掉NativeVlan|
|trunk|NativeVlan|无VLAN的报文,打上VLAN|剥掉NativeVlan|
|trunk|TaggedVlan||在芯片中属于TaggedVlan的报文, 该口转发|
|hybrid|NativeVlan|无VLAN的报文,打上VLAN|剥掉NativeVlan|
|hybrid|TaggedVlan||在芯片中属于TaggedVlan的报文, 该口可以转发|
|hybrid|UntaggedVlan||在芯片中属于TaggedVlan的报文, 该口可以转发, 转发后去掉Vlan|

+ 对于TaggedVlan和UntaggedVlan不是所以Vlan匹配的报文都转发, 报文dmac已知出端口 `!=` 该端口则不会转发
+ 所有报文进入交换机后肯定是有VLAN的, 要么是进入前就有的, 要么是进入后被带上的NativeVlan的
+ 报文出来交换机不一定有Vlan,被UntaggedVlan去掉了