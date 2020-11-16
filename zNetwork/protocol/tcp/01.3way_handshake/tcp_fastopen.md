[TOC]
# 1. 连接建立的场景
## 1.1. 首次建立连接的流程
1. client send tcp-syn, tcp头包含fastopen-option, cookie字段为空, 指示客户端开启了tcp_fastopen
2. server 接收到开启 tcp_fastopen 空 cookie 的 syn, 生成 cookie, 放到 syn-ack 种返回客户端
3. client 接收到带 cookie 的 syn-ack; 缓存到本地

## 1.2. 后序建立连接的流程 (cookie 有效) (比如一次 http 的 get/post 请求)
1. client 发送 { cookie + syn + data } 的报文到服务端
2. server 检验 cookie
3. server 返回 syn+ack, ack 包含data部分(指示client 发送窗口移动)
4. 且服务端进程 listen + recv 得到data
5. 服务端后序处理并返回 响应内容到客户端

## 1.3. 后序建立连接的流程 (cookie 无效)
1. client 发送 { cookie + syn + data } 的报文到服务端
2. server 检验 cookie
3. server 返回 syn+ack, ack 仅仅只是 client 的 syn
4. 客户端重发 data (窗口算法 ack 没确认); 且客户端删除 cookie 缓存
5. 下一次客户端建立连接; 重新走首次建立连接的流程