# 2. 四次握手
## 2.1 四次挥手 状态流1  (FIN - FIN - ACK - ACK) (两端同时close)
```log
主动端                                         主动端
close  @ ----- FIN -------> <---- FIN -------- # close
       @ ----- ACK -------> <---- ACK -------- #
实际上:

主动端                      主动端
close  @ ----- FIN -------> #
       @ <---- FIN -------- # close
       @ ----- ACK -------> #
       @ <---- ACK -------- #
```
1. 两端: FIN_WAIT_1 -> CLOSING -> TIME_WAIT -> CLOSED
  + 当两端同时发起关闭时; 即一段发起关闭, FIN报文还没到另一端时, 另一端也发起了关闭

## 2.2 四次挥手 状态流2  (FIN - ACK - FIN - ACK)
```log
主动关闭                         被动关闭
close       @ ----- FIN -------> #
FIN_WAIT_1  @                    #
            @ <---- ACK -------- # 这个是内核收到FIN后触发的; 然后用户态EIO
FIN_WAIT_2  @                    # CLOSE_WAIT
            @ <---- FIN -------- # 用户 close / shutdown
TIME_WAIT   @                    # LAST_ACK
            @ ----- ACK -------> #
CLOSED      @                    # CLOSED     收到最后的ACK后才进入CLOSED状态
```
1. 主动端: FIN_WAIT_1 -> FIN_WAIT_2 -> TIME_WAIT -> CLOSED
2. 被动端: CLOSE_WAIT -> LAST_ACK -> CLOSED

## 2.3 四次挥手 状态流3  (FIN - ACK&FIN - ACK)
```log
主动关闭                         被动关闭
close       @ ----- FIN -------> *
FIN_WAIT_1  @                    *
            @ <---- ACK FIN----- * 用户 close / shutdown
            @                    * CLOSE_WAIT -> LAST_ACK (短时间内切换)
            @ ----- ACK -------> *
TIME_WAIT   @                    * CLOSED     收到最后的ACK后才进入CLOSED状态
```

1. 主动端: FIN_WAIT_1 -> TIME_WAIT -> CLOSED
2. 被动端: CLOSE_WAIT -> LAST_ACK -> CLOSED
  + 被动端 内核把用户态的 ACK 和 FIN 合并到一起发了一个包