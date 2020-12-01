[TOC]
1. 参考 test/test-tcp-oob.c 例子 server 向客户端发 hello
2. 同时分析 tcp 和 stream; 因为 tcp 底层实现离不开 stream
    + 后面可能会单独分析 stream
3. 原生 tcp 
    1. server: socket, bind, listen, accept(阻塞), read(阻塞), write
    2. client: socket, bind, connect(阻塞), read(阻塞), write
4. libuv 的 tcp 用法
    1. server: uv_tcp_init, uv_tcp_bind, uv_listen(异步回调), uv_accept(同步的)
    2. client: uv_tcp_init, uv_tcp_connect(异步回调)
5. libuv 的 fd  操作
    1. 读: uv_read_start, uv_read_stop
    2. 写: uv_write,
    3. 关: uv_close, uv_shutdown

# 1. uv_tcp_init
1. 主要业务
    1. 系统调用 `socket()` 创建 fd
    2. 赋值 uv__io_t 回调 uv__stream_io
    3. 赋值 uv__io_t 的fd 刚刚创建的
    4. 置空 uv__io_t 的 epoll 事件
2. 调用关系
```js
// note: w->cb 虽然 uv_tcp_init 赋值为 uv__stream_io
//             但是 uv_tcp_listen 将被赋值为 uv__server_io (服务端)
//             所以 uv_tcp_connect 使用的还是 uv__stream_io (客户端)
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

# 2. uv_tcp_bind
1. 主要业务
    1. 系统调用 `bind()` 绑定监听的地址
2. 调用关系
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
1. 主要业务
    1. 系统调用 `listen()` 开启监听
    2. 赋值 tcp 层的用户回调 connection_cb
    3. 赋值 uv__io_t 层的回调 uv__server_io
    4. 传递 uv__io_t 层的监听事件 POLLIN 到 pevents
    5. 把 uv__io_t 对象加入 loop->watchers 和 loop->watcher_queue
2. 调用关系
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
    tcp->connection_cb = cb; // tcp 层的回调, 在 io_watcher.cb 中调用
    tcp->io_watcher.cb = uv__server_io; // io_watcher 的回调, uv_run 调用
    // // 把tcp连接对象, 加入到 事件轮的监听队列中
    uv__io_start(w = &tcp->io_watcher, events = POLLIN)
}
```
1. loop->watcher_queue 和 loop->watchers 将在 uv_run > uv__io_poll 中被遍历
   watcher_queue 节点 io_watcher 的 fd 通过 epoll_ctl 加入 loop->backend_fd
   并在 uv__io_poll 中使用 epoll_wait 监听事件

# 4. uv_accept
1. 主要业务 (server, client 都是 uv_tcp_t)
    1. 把 server->accepted_fd 取出来, 构造一个 client 传出对象
    2. 从 server->queued_fds 队列取出新的 accepted_fd 留作下次 uv_accept
    3. 维护 server->queued_fds 队列, (异步accept返回的队列, 实际上是 epoll_wait 返回的)
    4. 如果 server->queued_fds 为空了, 再次把 server->io_watcher.fd 加到
       事件轮的监听队列中, 等待下次 uv_run 加入 epoll_wait 监听
2. 调用关系
```js
uv_accept(server/*传入*/, client/*传出*/) {
    // 这里的 server, client  就是 uv_tcp_t
    >  uv__stream_open(stream = client, fd = server->accepted_fd) {
        // ... 错误检查; socket设置
        // 把传入的 fd, 传递给 uv_stream 对象;
        // 而该是 uv_stream 对象是新创建的 client 句柄
        stream->io_watcher.fd = fd;
    }
    // 维护 server->queued_fds 队列(异步accept返回的队列) (该队列是动态数组)
    // 1. 如果 queued_fds 队列非空
    //    2. 取出队列中的首个fd 到 server->accepted_fd (用以下次 uv_accept)
    //    3. 维护queue_fds 剩下的队列
    //    3.1 队列已经去空了: 释放掉 queued_fds
    //    3.2 队列还没有清空: 把后面的元素前移
    // 4. 如果 queued_fds 已经为空
    //    5. 干掉 accepted_fd
    //    6. 再次把 server->io_watcher.fd 加入到 事件轮的监听队列中 uv__io_start
    //       server->io_watcher.fd 就是 uv_tcp_init 创建的 fd
    if (server->queued_fds != NULL) { // 1.
        // 2.
        uv__stream_queued_fds_t* queued_fds = server->queued_fds; 
        server->accepted_fd = queued_fds->fds[0];
        // 3.
        assert(queued_fds->offset > 0);
        if (--queued_fds->offset == 0) {    // 3.1
            uv__free(queued_fds);
            server->queued_fds = NULL;
        } else {                            // 3.2
            memmove(queued_fds->fds, queued_fds->fds + 1, queued_fds->offset * sizeof(*queued_fds->fds));
        }
    } else { // 4.
        server->accepted_fd = -1; // 5.
        if (err == 0)
            uv__io_start(server->loop, &server->io_watcher, POLLIN); // 6.
    }
}
```

# 5. uv_tcp_connect
1. 主要业务
    1. 系统调用 `connect()` 传入连接地址
    2. 构造 uv__req_t 对象并传递 tcp->connect_req
    3. 传递用户回调 到 uv__req_t 对象
    4. 传递 uv__io_t 层的监听事件 POLLOUT 到 pevents
    5. 把 uv__io_t 对象加入 loop->watchers 和 loop->watcher_queue
2. 调用关系
```js
uv_tcp_connect(handle, addr, cb) > uv__tcp_connect(handle, addr, cb) {
    // 这里的 handle 就是 uv_tcp_t
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
    // 底层 connect 系统调用
    // 这里是异步的, 即马上返回
    do {
        errno = 0;
        r = connect(uv__stream_fd(handle), addr, addrlen);
    } while (r == -1 && errno == EINTR);

    // 初始化 req 对象, 类型为 UV_CONNECT
    uv__req_init(handle->loop, req, UV_CONNECT);
    req->cb = cb; // 传递用户态回调
    req->handle = (uv_stream_t*) handle; // 传递 uv_stream 对象
    QUEUE_INIT(&req->queue); 
    handle->connect_req = req; // 放到 uv_stream 对象->connect_req 队列  中

    uv__io_start(handle->loop, &handle->io_watcher, POLLOUT);
}
```
# 6. uv_read_start uv_read_stop
# 7. uv_write
# 8. uv_shutdown
# 9. uv_close

# 11. uv_tcp_t
```c++
struct uv_tcp_t {
// UV_HANDLE_FIELDS
    uv_loop_t* loop;     // 事件轮
    uv_handle_type type; // 多态标记
    int fd; // 1. tcp 服务端 listen 的fd
            // 2. tcp 服务端 accept 的fd, 即完成三次握手的连接
            // 3. tcp 客户端 connect 的fd

// UV_STREAM_FIELDS
    uv_connect_t *connect_req;      // 连接请求; 内有 用户回调 (uv_tcp_connect)
    uv_shutdown_t *shutdown_req;    // 关闭请求; 内有 用户回调 (uv_shutdown)
    uv__io_t io_watcher;            // stream 层对象
    void* write_queue[2];
    void* write_completed_queue[2];
    uv_connection_cb connection_cb; // 建立连接; 指向 用户回调 (uv_tcp_listen)
    int delayed_error;
    int accepted_fd;             // 最早 accept 用户还没处理的 fd (可以认为是变长数组的头)
    void* queued_fds;            // 后续 accept 用户还没处理的 fd (变长数组)
}
```
# 12. uv__io_t
```c++
struct uv__io_t {
    uv__io_cb cb; // uv__io 层的回调
                  // 1. tcp 统一地 uv_tcp_init   初始化 uv__stream_io
                  // 2. tcp 服务端 uv_tcp_listen 重写为 uv__server_io
                  // 3. tcp 其他操作 uv_accept, uv_connect, uv_read, uv_write
                  //        统一继承 uv__stream_io
    void* pending_queue[2];
    void* watcher_queue[2]; // 连接 loop->watcher_queue 事件轮的队列
    unsigned int pevents; // 还没通知内核 epoll 的事件
                          // POLLOUT: uv_tcp_connect
                          // POLLIN:  uv_tcp_listen
    unsigned int events;  // 已经通知内核 epoll 的事件
    int fd;       // 因为 uv_run 操作的是 uv__io 层
                  // 所以 要把 uv_tcp_t 的 fd 复制一份到这里来
};
```


# 91. uv__io_start (io对象 新增事件 and 入列 loop->watcher_queue)
1. 主要业务
    1. 传递 events 参数到 uv__io_t 对象的 pevents
    2. 把 uv__io_t 对象加入到 watcher_queue 和 watchers
2. 调用关系
```js
uv__io_start(w = &handle->io_watcher, events) {
    // ...
    // epoll 的 事件参数 传递
    w->pevents |= events;
    // 把io对象, 加入到 事件轮的监听队列中
    if (QUEUE_EMPTY(&w->watcher_queue))
        QUEUE_INSERT_TAIL(&loop->watcher_queue, &w->watcher_queue);
    // 把io对象的 fd, 加入到 事件轮的监听fd数组中 (fd即索引)
    if (loop->watchers[w->fd] == NULL) {
        loop->watchers[w->fd] = w;
        loop->nfds++;
    }
}
```
# 92. uv__io_stop (io对象, 删除事件 and 出列 loop->watcher_queue)
1. 主要业务
    1. 干掉 uv__io_t 对象的 pevents 中的指定事件
    2. 如果没有事件, 销毁 
```js
uv__io_stop(w = &handle->io_watcher, events) {
    // ...
    // epoll 的事件传递, 先干掉事件
    w->pevents &= ~events;
    // 如果没有事件了, 那就从 事件轮的监听队列 中干掉
    if (w->pevents == 0) {
        QUEUE_REMOVE(&w->watcher_queue); // 出列
        QUEUE_INIT(&w->watcher_queue);   // 初始化双向链表的节点

        // 维护事件轮的 fd 表
        if (loop->watchers[w->fd] != NULL) { 
            loop->watchers[w->fd] = NULL;
            loop->nfds--;
            w->events = 0;
        }
    } 
    // 如果还有事件, 但已经出列, 那就重新入列
    else if (QUEUE_EMPTY(&w->watcher_queue))
        QUEUE_INSERT_TAIL(&loop->watcher_queue, &w->watcher_queue);
}
```
# 93. uv__server_io 服务端 uv_stream 层回调
```js
uv__server_io(w /* loop->watch_queue 节点 */, events) {
    // 获取 uv_stream 对象
    stream = container_of(w, uv_stream_t, io_watcher);
    // 再次start; 因为从 uv_run 跑进来的时候, 从队列中被干掉了
    uv__io_start(stream->loop, &stream->io_watcher, POLLIN);
    // 因为fd 有可能在回调中被干掉, 所以accept过程中需要感知
    while (uv__stream_fd(stream) != -1) {
        err = uv__accept(uv__stream_fd(stream)); // 底层 accept 系统调用
        if (err < 0) {
            // ... err 错误检查
            // 出错了, 提前通知上层用户, 这里是 uv_tcp_listen 用户回调
            stream->connection_cb(stream, err);
            continue;
        }
        // 把 accept 到的fd 传递到 accepted_fd
        stream->accepted_fd = err; 
        // 正常通知上层用户, 这里是 uv_tcp_listen 用户回调
        stream->connection_cb(stream, 0);

        // a) 一般地 uv_tcp_listen 用户回调中
        // b) 会使用 uv_accept, 取出 accepted_fd 作为客户端对象,
        // c) 取完后 accepted_fd 置为 -1 (uv_accept 的逻辑)
        // d) 如果用户没有取走, 则先干掉 POLLIN 事件
        //    使得后续的 uv_run 不监听三次握手成功的连接,
        //    但连接还是会放到内核态的三次握手队列中
        // e) 等后面用户取走后, 再次加入 loop->watcher_queue 中 (uv_accept 的逻辑)
        if (stream->accepted_fd != -1) {
            uv__io_stop(loop, &stream->io_watcher, POLLIN);
            return;
        }
    }
}
```

# 94. uv__stream_io 综合性 uv_stream 回调, 处理 connect/read/write
```js
uv__stream_io(w /* loop->watch_queue 节点 */, events) {
    uv_stream_t* stream = container_of(w, uv_stream_t, io_watcher);
    // 处理 connect
    if (stream->connect_req) {
        > uv__stream_connect(stream) > {
            if (req->cb) // uv_tcp_connect 的用户回调
                req->cb(req, error);
        }
        return;
    }
    // 处理 read
    if (events & (POLLIN | POLLERR | POLLHUP)) {
        > uv__read(stream) > {
            if (!is_ipc) { // 底层 read 系统调用
                nread = read(uv__stream_fd(stream), buf.base, buf.len);
            } else {       // 底层 recvmsg 系统调用
                nread = uv__recvmsg(uv__stream_fd(stream), &msg, 0);
            }
            stream->read_cb(); // uv_read 的用户回调
        }
    }
    // 处理 write
    if (events & (POLLOUT | POLLERR | POLLHUP)) {
        uv__write(stream);
        > uv__write_callbacks(stream) {
            // ...
            if (req->cb)
                req->cb(req, req->error); // uv_write 的用户回调
        }

        if (QUEUE_EMPTY(&stream->write_queue)) // 耗尽 write_queue
            uv__drain(stream);
    }
}
```
# 95. uv__stream_connect
# 96. uv__read
# 97. uv__write