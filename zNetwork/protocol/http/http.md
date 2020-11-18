[TOC]
https://www.cnblogs.com/xiaolincoding/p/12442435.html

1. http 超文本传输协议
    1. 协议: 计算机间 数据的{ 通讯/控制/错误处理 } 的约定和规范
    2. 传输: 两点之间双向传输数据约定和规范
    3. 超文本: 文字/图片/音频/视频等"超文本"数据的约定和规范
# 1. 状态码
## 1xx 提示信息, 指示协议中间状态
## 2xx success, 报文被正确处理
1. 200 OK: 一切正常. 如果是非 HEAD 请求，服务器返回的响应头都会有 body 数据
2. 204 No Contnet: 同于 200 OK, 但响应 header 没有 body
3. 206 Partial Content:  HTTP 分块下载或断点续传
    1. 响应返回的 body 数据并不是资源的全部,
    2. 后续还有资源
    3. 但服务器处理成功
## 3xx redicrect, resource position 变动, client 需要重新request
1. 301 Moved Permanently: 永久重定向; resource 已经不存在, 需要用另一个 url 再次、访问
    + 响应 header 的 Location: 指示新的 url
2. 302 Found: 临时重定向; resource 还在, 但暂时需要用另一个 url 访问
    + 响应 header 的 Location: 指示新的 url
3. 304 Not Modified: 缓存重定向; 不表示跳转; 但重定向 至 已缓存的文件
    + 用于缓存控制
## 4xx client error, request 有误, server 无法处理
1. 400 Bad Request: 笼统的错误 (形同500)
2. 403 Forbidden: 禁止访问资源; request 报文无误, 但 recource 不允许 access
    + 资源是存在的
3. 404 Not Found: request 的 resource 找不到, 无法返回给客户端
    + 资源不存在
## 5xx server error, server 在 response 过程中发生内部错误
1. 500 Internal Server Error: 笼统的错误 (形同400)
2. 501 Not Implemented: client request 的功能还不支持
    + 能识别出来, 但不支持
3. 502 Bad Gateway: server 作为 gateway or proxy 时, 本身工作正常, 但访问下一跳出错了
    + client <---> server(proxy) <---> server(real)
    + server(proxy) 正常, server(real) 出错
4. 503 Service Unavailable: server 正忙, 稍后重试

# 2. 字段
1. `Host: www.baidu.com`                        // 指示服务器域名
2. `Content-Length: 1000`                       // 本次回应的数据长度
3. `Connection: keep-alive`                     // http tcp 长连接建立
    1. 默认 http tcp 连接在一次连接建立后, 马上销毁
    2. 这个字段配合 tcp_fastopen 使用 (绕过三次握手)
4. `Content-Type: text/html; charset=utf-8`     // 服务端返回时指示数据格式
5. `Accept: */*`                                // 客户端指示支持的数据格式
6. `Content-Encoding: gzip`                     // 服务端返回时指示压缩格式
7. `Accept-Encoding: gzip, deflate`             // 客户端指示支持的压缩格式

# 3. GET and POST 请求方法
1. GET:  从服务器获取资源 (文本 / 图片 / 音频 / 视频)
    + 只读操作
2. POST: 向 URI 指定的资源, commit 数据(在 body 中)
    + 读写操作
3. Q: GET 和 POST 方法都是 "安全"和 "幂等" 的吗?
    1. 安全 = 请求求方法不会修改服务器上的资源 
    2. 幂等 = 重复执行相同的操作，结果都是"相同"的
    3. GET 是 安全和幂等的, 因为是只读操作, 重复多次也不会修改资源 and 结果相同
    4. POST 不安全和不幂等, 可能改写资源, 且多次操作结果肯定不同

# 4 HTTP 1.1 特性
## 4.1 优点
1. 简单: 
    1. 报文格式就是 header + body
    2. header 信息就是 key-value
2. 灵活和易扩展
    1. 请求方法, URI/URL, 状态码, header-key 都允许自定义和扩充
3. 无状态: server 不记忆HTTP状态, 节省资源
## 4.2 缺点
1. 无状态: 对于事务性的状态迁移类业务, 处理起来比较麻烦 (如账户的每次操作都要 登录验证)
    1. 规避: 使用 Cookie 记录状态
2. 明文传输: 信息被窃听导致泄漏
3. 不安全:
    1. 明文通讯:   窃听风险
    2. 无身份验证: 冒充风险
    3. 无完整性校验: 篡改风险
## 4.3 性能
1. 长连接: 避免 http1.0 每次请求都要建立三次握手; 一旦握手成功, 后续请求不再握手
2. 管道网络传输: 客户端可同时发出多次请求, 但服务器还是得逐个响应, 减少整体的响应时间
    + 形同 tcp 的滑动窗口
3. 队头阻塞: 请求-响应 的模型中, 如果单次请求服务器处理时间过长, 将导致后续请求阻塞
    + 即使管道网络传输, 服务器处理多个响应不是并发的, 是串行的

# 5. https
## 5.1 混合加密
1. 通信建立前(握手阶段) **非对称加密** 交换 *会话秘钥*
    1. client request server 公钥
    2. client 用 server response 的公钥加密报文
    3. server 使用自己的私钥解密报文
2. 通讯建立后(请求响应) 使用 *会话秘钥* 进行 **对称加密**
    + 对称加密运算简单, 相对较快, 但安全性低
## 5.2 摘要算法
1. 即在计算数据报文的**摘要**(如 md5sum), 然后放到明文数据中 进行加密传输
2. 接收端检验报文的**数据**和**摘要**
## 5.3 身份证书
1. 为了保证 client 获取到 server 给的公钥 不被篡改
2. 服务器把自己的公钥注册到 CA中
3. CA 把 server 的公钥 使用 *CA私钥* 加密, 颁发数字证书给 server
4. client 从 CA 获取 *CA公钥* 缓存到本地
5. client 获取到 server 给的数字证书, 使用 *CA公钥* 确认 server公钥
## 5.4 通讯建立
1. 客户端向服务器 request 公钥, 并通过数字证书校验 (非对称加密)
    + ssl/tls 建立过程a)
2. 双方协商产生 *会话秘钥*
    + ssl/tls 建立过程b)
3. 双方使用 *会话秘钥* 加密通讯 (对称加密)


# 6. http 与 https 对比
## 6.1 http 与 https 的区别
1. 数据加密: http 是明文传输, https 在 tcp 和 http 间使用 SSL/TLS 进行数据加密
2. 双重握手: http 只需要tcp三次握手, https 还需要 ssl/tls 的握手
3. 端口号:   http 是 80, https 是 443
4. 身份验证: https 的服务器, 需要向 CA (证书权威机构) 申请数字证书 (保证服务器 身份可信)
## 6.2 https 解决了 http 的哪些缺陷? 如何解决?
1. http 的缺陷来源于明文传输
    1. 窃听风险
    2. 冒充风险
    3. 篡改风险
2. https 数据加密弥补了缺陷
    1. 信息加密: "对称加密"和"非对称加密"结合的混合加密，解决窃听的风险
    2. 校验机制: "摘要算法"用来实现 报文完整性校验, 防止篡改
    3. 身份证书: 服务器公钥放在数字证书(CA 颁发)中, 只要证书是可信的, 公钥就是可信的

# 7. http 1.0/1.1/2.0/3.0 的演变
https 是基于 1.1 上做的, 即后续 http 2.0 3.0 继承了 ssl/tls
## 7.1 http 1.1 针对1.0 的优化
1. 使用 tcp 长连接减少避免每次 请求-响应 都要重新三次握手的问题
2. 使用 pipeline 网络传输, 减少整体的响应时间
    1. 但 client 仍然是串行 request  的
    2. 但 server 仍然是串行 response 的 (存在 队头阻塞 问题)
## 7.2 http 2.0 针对1.1 的优化
1. header 头部压缩(hpack 算法): 如果连续发出多个请求, header 是一样or相似的, 将"去重"
    1. 双方维护 header 的 { idx: key-value } table
    2. 如果后续请求 key-value, 相同, 只发送 idx 替代
2. 二进制格式: header 和 body 部分使用二进制, 称为 frame; 即 header-frame, body-frame
    1. 二进制流能提高传输效率
3. 数据流:
    1. 数据部分再是按顺序发送, 可能是同时乱序发送
    2. 引入优先级, 优先级高的 request, server 优先处理 (这需要 双方支持)
4. 多路复用: 一个tcp数据中 嵌套多个 request or response 
    1. 避免 队头阻塞 问题
5. 服务器推送 (server push) or (cache push)
    1. 不仅支持 请求-响应 这种被动响应模式
    2. 额外支持 server 主动地向 client 发送数据 (基础是 tcp 长连接)
## 7.3 http 3.0 针对2.0 的优化
1. 把 tcp 改为 udp (实际上是基于 udp 的 quic 协议)
    1. http/2.0 的多路复用, 在tcp部分丢包时(滑动窗口 前部分丢包), 后续请求将被阻塞
2. quic 连接可靠性:
    1. quic 自身的连接可靠性机制
    2. 在多路复用时, 因为不是 stream, 没有滑动窗口, 前部分丢包, 不阻塞后续请求
3. qpack 算法: 新的 header 压缩算法
4. 压缩 **tcp三次握手** + **ssl/tls 四次握手** => quic3次握手

# 8. ssl / tls 4次握手
```txt
client                 server
   *-------- syn --------->
   <----- syn + ack ------*
   *-------- ack --------->
   
   *---- client hello ---->  不加密
   <---- server hello ----*  不加密 (其实 CA的数字证书 本身已经加密过了)

   *--- client response -->  使用 server 公钥加密/解密
   <--- server response --*  使用 server 公钥加密/解密

   *----- GET / POST ----->
```
## 8.1 client hello
1. tcp 三次握手后, client 发起加密通讯
2. 主要发送
    1. ssl/tls version (tls 1.2)
    2. client random number (用于产生 *会话秘钥*)
    3. client 支持的密码套件 list, 如 RSA
## 8.2 server hello
1. 服务端收到 client hello 后, 进行响应
2. 主要发送
    1. 确认 ssl / tls, 如果 client 不支持, 则需要关闭连接
    2. server random number (用于产生 *会话秘钥*)
    3. server 支持的密码套件 list, 如 RSA
    4. 服务器的数字证书 (client 校验失败 则关闭连接)
## 8.3 client response
1. client 从 服务器的数字证书 提取 server 公钥
2. 主要发送(server公钥 加密)
    1. 加密通信算法改变通知, 指示后续通讯使用 *会话秘钥*
    2. pre-master key (随机数), 数值本身还需要server公钥 加密
    3. client 握手结束标记
    4. 摘要{ client hello, server hello, client response }
## 8.4 server response
1. server 用 pre-master key + client server + server random number =生成=> *会话秘钥*
2. 主要发送(server公钥 加密)
    1. 加密通信算法改变通知, 指示后续通讯使用 *会话秘钥*
    2. server 握手结束标记
## 8.5 request and  response
1. 双方使用 *会话秘钥* 通讯