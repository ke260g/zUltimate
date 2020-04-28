1. mac-forward-table is shared by multi asic on a single switch
2. on stack-mode, multi-switch's mac-forward-table is synchronize
3. using modid to indicate which asic is


Port:
## 1. learning state
1. Learn:      allow to learn into L2 table automatically
               if set; can be learned by software manually.
2. ToCPU:      allow copy to CPU automatically
3. Forwarded:  allow to forward automatically
               (is  这个口入方向报文, 可以自动转发)
               (not 其他口的报文, 可以转发到这个口)
4. Discard:    packet will be discarded. But can be recv by acl.
               if set; can be get by acl only.
+ 1 2 3 是并行的
+ 4会使得1 2 3失效; 只要1/2/3一个开启, 4必须为0;

## 2. stp state:
### Block  (一般用于 避免环路)
1. 特性
  1. 其他口进来的报文: 不许自动从这个口 自动转发 
  2. 这个口进来的报文: 不允许自动转发
  3. 但是, 可以通过acl匹配, 然后copytocpu
2. 已知场景:
  1. MAD, STP, 故意环路的拓扑, 不允许 SDK层环路(耗尽CPU)
  2. 单端口防环, 
### Forward: 
1. 特性
  1. 其他口进来的报文; 允许自动从这个口 自动转发
