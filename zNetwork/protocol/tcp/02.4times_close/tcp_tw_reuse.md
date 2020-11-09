[TOC]
# 复用流程1
1. `connect()` 方复用 socket; 发送 syn
2. `listen()`  方上一个连接已经脱离 LAST_ACK
3. 连接正常建立

# 复用流程2
1. `connect()`端 复用 socket; 发送 syn
2. `listen()` 端 上一个连接还处于 LAST_ACK；检查 syn 的 timestamps 后丢弃 
3. `listen()` 端 在 LAST_ACK 状态中重复 FIN；而 `connect()`方处于 SYN_SENT
4. `connect()`端 SYN_SENT 状态中收到 FIN 会返回 RST 给 `listen()` 端
5. `listen()` 端 最终脱离 LAST_ACK 进入CLOSED
6. `listen()` 端 最终收到 SYN 走正常的三次握手
```log
      connect()端                    listen() 端
       SYN_SENT @ ----- SYN -------> * LAST_ACK (timestamps 丢包)
tcp_syn_retries @ ----- SYN -------> * LAST_ACK (timestamps 丢包)
                @ .................. *
                @ <---- FIN -------- * tcp_orphan_retries
                @ ----- RST -------> * LAST_ACK
                @ .................. * CLOSED
tcp_syn_retries @ ----- SYN -------> * CLOSED
                  ..................
                  正常的三次握手过程
```