[TOC]
1. 参考 test/test-tcp-oob.c 例子 server 向客户端发 hello
2. 同时分析 tcp 和 stream; 因为 tcp 底层实现离不开 stream
3. 原生 tcp 
    1. server: socket, bind, listen, accept(阻塞), read(阻塞), write    u bxcgy
    2. client: socket, bind, connect(阻塞), read(阻塞), write

# 1. uv_tcp_init
1. 调用关系
```js
uv_tcp_init() > uv_tcp_init_ex() {
    > uv__stream_init(UV_TCP) {
        // 多态初始化; 类型 UV_TCP
        > uv__handle_init(stream, UV_TCP) { 
            // 初始化 写队列
            QUEUE_INIT(&stream->write_queue);
            QUEUE_INIT(&stream->write_completed_queue);
        }
        // 初始化 io_watcher 对象
        > uv__io_init(&stream->io_watcher, uv__stream_io, -1) {
            QUEUE_INIT(&w->pending_queue);
            QUEUE_INIT(&w->watcher_queue);
            w->cb = cb;  // 传入参数: uv__stream_io 回调
            w->fd = fd;  // 传入参数: -1
            w->events = 0;
            w->pevents = 0;
        }
    }

    > maybe_new_socket() {
        // 如果 fd 已经创建; 首次初始化还没创建
        // #define uv__stream_fd(handle) ((handle)->io_watcher.fd)
        if (uv__stream_fd(handle) != -1) { /* ... */ }
        // 2. 创建新的fd
        > new_socket() {
            // 系统调用 socket()
            sockfd = uv__socket(domain, SOCK_STREAM, 0);
             // 把 fd 传回 stream 句柄
            > uv__stream_open(sockfd) {
                stream->io_watcher.fd = fd;
            }
            // 还没有 UV_HANDLE_BOUND 先不管; 后面要用 uv_tcp_bind
            if (flags & UV_HANDLE_BOUND) {
                bind(uv__stream_fd(handle), (struct sockaddr*) &saddr, slen)
            }
        }
    }
}
```
2. 主要业务: 创建了一个 fd, 初始化 stream 对象

# 2. uv_tcp_bind
```js
uv_tcp_bind(addr) > uv__tcp_bind(addr) {
    > maybe_new_socket(addr) {
        // 这里 fd 已经创建了; 之前 uv_tcp_init 创建
        // #define uv__stream_fd(handle) ((handle)->io_watcher.fd)
        if (uv__stream_fd(handle) != -1) {
            // flags 为 0
            if (flags & UV_HANDLE_BOUND) { /* ... */ }
            // 直接返回
            return 0;
        }
    }
    // 系统调用 bind
    > bind(tcp->io_watcher.fd, addr, addrlen)
    // 拉起 UV_HANDLE_BOUND 标记; 指示该 socket 已经 bind 了
    tcp->flags |= UV_HANDLE_BOUND;
}
```

# 3. uv_listen
```js
uv_listen() > uv_tcp_listen(stream, backlog, cb) {
    > maybe_new_socket(addr) {
        // 这里 fd 已经创建了; 之前 uv_tcp_init 创建
        // #define uv__stream_fd(handle) ((handle)->io_watcher.fd)
        if (uv__stream_fd(handle) != -1) {
            // flags 为 0
            if (flags & UV_HANDLE_BOUND) { /* ... */ }
            // 直接返回
            return 0;
        }
    }
    // 系统调用 listen
    > listen(tcp->io_watcher.fd, backlog)
    tcp->io_watcher.cb = uv__server_io // io_watcher 的回调
    > uv__io_start(w = &tcp->io_watcher, events = POLLIN) {
        // epoll 的 事件参数 传递
        w->pevents |= events;
        // 把tcp连接对象, 加入到 事件轮的监听队列中
        if (QUEUE_EMPTY(&w->watcher_queue))
            QUEUE_INSERT_TAIL(&loop->watcher_queue, &w->watcher_queue);
        // 把tcp监听fd  , 加入到 事件轮的监听数组中, fd 作为索引
        if (loop->watchers[w->fd] == NULL) {
            loop->watchers[w->fd] = w;
            loop->nfds++;
        }
    }
}
```

# 4. uv_accept
# 5. uv_tcp_connect
# 6. 
# 7. 