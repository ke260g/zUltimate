[TOC]
# 1. 对象定义 typedef struct uv_loop_s uv_loop_t 
```c++
struct uv_loop_s {
  void* data;     // 用户句柄
// UV_LOOP_PRIVATE_FIELDS
  int backend_fd; // epoll_create 返回的 fd

  void* pending_queue[2];
  void* watcher_queue[2]; // tcp, udp 连接的 uv__io_t 对象队列
  uv__io_t** watchers;    // watcher_queue 的 fd 索引, 即 watchers[fd] 为一个 uv__io_t 指针
                          //   a) 这里的 fd 就是 epoll_ctl 加入到 backend_fd 
                          //   b) 也是 epoll_wait 传出的 fd
                          // 这样设计的理由是:
                          //   1) 当某个回调中销毁掉了 tcp/udp/uv__io_t 对象, 此时 watchers[fd] 就会销毁,
                          //   2) 但是, 销毁是异步进行的, 即底层 epoll_ctl 退出 backend_fd 延时到 uv_run
                          //   3) 所以, 当 epoll_wait 返回fd, 但 watchers[fd] 为空, 才 epoll_ctl 进行退出
  unsigned int nwatchers; // watchers 变长数组的大小
  unsigned int nfds;      // 有效 fd 的个数 (watchers 中的有效fd个数)

  struct {        // 定时器 变长数组
    void* min;
    unsigned int nelts;
  } timer_heap;

  void* prepare_handles[2]; // uv_prepare_t 队列; 双成员数组实现链表
  void* check_handles[2];   // uv_check_t 队列; 双成员数组实现链表
  void* idle_handles[2];    // uv_idle_t 队列; 双成员数组实现链表
};
typedef struct uv_loop_s uv_loop_t;
```

# 2. 对象获取 uv_default_loop uv_loop_new
```c++
// 默认事件轮 default_loop_ptr (每个进程一个)
static uv_loop_t default_loop_struct;
static uv_loop_t* default_loop_ptr;
uv_loop_t* uv_default_loop(void) {
  if (default_loop_ptr != NULL)
    return default_loop_ptr;

  if (uv_loop_init(&default_loop_struct))
    return NULL;

  default_loop_ptr = &default_loop_struct;
  return default_loop_ptr;
}

// 创建一个新事件轮
// 分配内存再初始化
uv_loop_t* uv_loop_new(void) {
  uv_loop_t* loop;
  loop = uv__malloc(sizeof(*loop)); // 分配内存
  if (loop == NULL)
    return NULL;
  if (uv_loop_init(loop)) {         // 再初始化
    uv__free(loop);
    return NULL;
  }
  return loop;
}
```

# 3. 初始化  uv_loop_init
```c++
int uv_loop_init(uv_loop_t* loop) {
  // loop->data 是上层用户传进来的; 初始化要保留
  saved_data = loop->data; 
  memset(loop, 0, sizeof(*loop));
  loop->data = saved_data;

  // 不重要
  lfields = (uv__loop_internal_fields_t*) uv__calloc(1, sizeof(*lfields));
  if (lfields == NULL)
    return UV_ENOMEM;
  loop->internal_fields = lfields;

  // 这是啥功能? 为啥要加锁?
  err = uv_mutex_init(&lfields->loop_metrics.lock);
  if (err)
    goto fail_metrics_mutex_init;

  // 定时器的存储结构是 以超时时间排序的 小顶堆
  // 即 堆顶的超时时间 是最小的; 即将来最快触发
  heap_init((struct heap*) &loop->timer_heap);

  // 队列底层实现 void *queue[2]; 的双向链表
  // queue[0] 指向 prev; queue[1] 指向 next; 
  QUEUE_INIT(&loop->wq);
  QUEUE_INIT(&loop->idle_handles);    // 每次轮回都会执行; prepare 之前
  QUEUE_INIT(&loop->async_handles);
  QUEUE_INIT(&loop->check_handles);   // 每次轮回都会执行; io 之后
  QUEUE_INIT(&loop->prepare_handles); // 每次轮回都会执行; io 之前
  QUEUE_INIT(&loop->handle_queue);

  loop->active_handles = 0;
  loop->active_reqs.count = 0;
  loop->nfds = 0;
  loop->watchers = NULL;
  loop->nwatchers = 0;
  QUEUE_INIT(&loop->pending_queue);
  QUEUE_INIT(&loop->watcher_queue);

  loop->closing_handles = NULL; // 事件轮销毁回调
  uv__update_time(loop);
  loop->async_io_watcher.fd = -1;
  loop->async_wfd = -1;
  loop->signal_pipefd[0] = -1;
  loop->signal_pipefd[1] = -1;
  loop->backend_fd = -1; // 事件轮终极 fd; uv__platform_loop_init 初始化
  loop->emfile_fd = -1;

  loop->timer_counter = 0;
  loop->stop_flag = 0;
  // 这里初始化底层fd, linux 是 epoll, darwin 是 kqueue
  err = uv__platform_loop_init(loop);
  if (err)
    goto fail_platform_init;

  uv__signal_global_once_init();
  err = uv_signal_init(loop, &loop->child_watcher);
  if (err)
    goto fail_signal_init;

  uv__handle_unref(&loop->child_watcher);
  loop->child_watcher.flags |= UV_HANDLE_INTERNAL;
  QUEUE_INIT(&loop->process_handles);

  err = uv_rwlock_init(&loop->cloexec_lock);
  if (err)
    goto fail_rwlock_init;

  err = uv_mutex_init(&loop->wq_mutex);
  if (err)
    goto fail_mutex_init;

  err = uv_async_init(loop, &loop->wq_async, uv__work_done);
  if (err)
    goto fail_async_init;

  uv__handle_unref(&loop->wq_async);
  loop->wq_async.flags |= UV_HANDLE_INTERNAL;

  return 0;
  // 后面是错误处理
}
```

# 4. 事件轮底层fd实现 uv__platform_loop_init 
## 4.1 linux 架构, 使用 epoll 实现底层fd
`src/unix/linux-core.c`
```c++
int uv__platform_loop_init(uv_loop_t* loop) {
  int fd;
  fd = epoll_create1(O_CLOEXEC); // 这个可能失败; 内核不一定实现
  if (fd == -1 && (errno == ENOSYS || errno == EINVAL)) {
    fd = epoll_create(256);

    if (fd != -1) // 设置 O_CLOEXEC 标记
      uv__cloexec(fd, 1);
  }

  loop->backend_fd = fd;
  // ...
  return 0;
}
```
## 4.2 darwin 架构 使用 kqueue 实现底层fd
`src/unix/darwin.c` 
```c++
int uv__kqueue_init(uv_loop_t* loop) {
  loop->backend_fd = kqueue(); // darwin 架构
  if (loop->backend_fd == -1)
    return UV__ERR(errno);
  // 设置 O_CLOEXEC 标记
  uv__cloexec(loop->backend_fd, 1);
  return 0;
}

int uv__platform_loop_init(uv_loop_t* loop) {
  loop->cf_state = NULL;
  if (uv__kqueue_init(loop))
    return UV__ERR(errno);
  return 0;
}
```

# 5. 遍历 uv_run
```c++
int uv_run(uv_loop_t* loop, uv_run_mode mode) {
  // ...
  while (r != 0 && loop->stop_flag == 0) {
    uv__update_time(loop); // 使用 uv__hrtime 更新 loop->time 
    uv__run_timers(loop);  // 遍历 定时器
    ran_pending = uv__run_pending(loop);
    uv__run_idle(loop);    // 遍历 idle
    uv__run_prepare(loop); // 遍历 prepare
    // ...
    uv__io_poll(loop, timeout); // epoll_ctl 监听
    // ...
    uv__run_check(loop);  // 遍历 check
    // ...
  }
  // ...
  return r;
}
```

# 6. 监听 uv__io_poll
```c++
void uv__io_poll(uv_loop_t* loop, int timeout) {
  // ...
  struct epoll_event events[1024];
  struct epoll_event* pe;
  struct epoll_event e;
  int real_timeout;
  QUEUE* q;
  uv__io_t* w;
  sigset_t sigset;
  uint64_t sigmask;
  uint64_t base;
  int have_signals;
  int nevents;
  int count;
  int nfds;
  int fd;
  int op;
  int i;
  int user_timeout;
  int reset_timeout;

  if (loop->nfds == 0) {
    assert(QUEUE_EMPTY(&loop->watcher_queue));
    return;
  }

  memset(&e, 0, sizeof(e));
  // 把 loop->watcher_queue 中 fd 以及 需要监听的事件
  // 传入到 loop->backend_fd 中, 使用 epoll_ctrl 调用
  // 1. 遍历 loop->watcher_queue
  // 2. 节点出列
  // 3. 把节点 w->fd, w->pevents 传入 loop->backend_fd
  while (!QUEUE_EMPTY(&loop->watcher_queue)) {
    q = QUEUE_HEAD(&loop->watcher_queue);
    QUEUE_REMOVE(q);
    QUEUE_INIT(q);

    w = QUEUE_DATA(q, uv__io_t, watcher_queue);
    assert(w->pevents != 0);
    assert(w->fd >= 0);
    assert(w->fd < (int) loop->nwatchers);

    // w->pevents 是 libuv watcher 监听的事件; pending 表示还没传入内核
    // w->events  是 libuv watcher 监听的事件; 已经传入给内核
    // e.events   是 内核 epoll    监听的事件, 本次传入给内核
    e.events = w->pevents;
    e.data.fd = w->fd;

    if (w->events == 0)
      op = EPOLL_CTL_ADD;
    else
      op = EPOLL_CTL_MOD;

    // epoll_ctl,
    if (epoll_ctl(loop->backend_fd, op, w->fd, &e)) {
      if (errno != EEXIST)
        abort();

      assert(op == EPOLL_CTL_ADD);

      /* We've reactivated a file descriptor that's been watched before. */
      if (epoll_ctl(loop->backend_fd, EPOLL_CTL_MOD, w->fd, &e))
        abort();
    }
    // 已经传入给内核后, 记录到 w->events
    w->events = w->pevents;
  }
  // ...

  // 1. epoll_wait 监听 loop->backend_fd, 传出本次监听结果 events, 返回值 nfds 表示有事件的fd个数
  // 2. nfds == 0  { 本次没有事件, a) 如果还没超时 continue; }
  // 3. nfds == -1 { 本次监听失败, a) 如果还没超时 continue; b) 如果被中断 EINTR continue;  }
  // 4. 遍历 events
  // 4.1 如果 events->fd 已经不在 loop->watchers[fd] 中 (可能是其他业务干掉的)
  //    则把 fd 从 loop->backend_fd 干掉 epoll_ctl(EPOLL_CTL_DEL)
  // 4.2 pe->events &= w->events, 即只关注上层 watcher 关注的事件
  // 4.3 如果有事件 a) 信号事件 则 拉起标记 b) 其他事件 则 执行回调
  // 5. 如果有信号事件, 之前遍历 events 被拉起标记
  //    则执行 信号回调 signal_io_watcher.cb
  // 6. 超时退出
  // 7. 时间更新
  for (;;) {
    // ...
    if (no_epoll_wait != 0 || (sigmask != 0 && no_epoll_pwait == 0)) {
      nfds = epoll_pwait(loop->backend_fd,
                         events,
                         ARRAY_SIZE(events),
                         timeout,
                         &sigset);
      if (nfds == -1 && errno == ENOSYS) {
        uv__store_relaxed(&no_epoll_pwait_cached, 1);
        no_epoll_pwait = 1;
      }
    } else {
      nfds = epoll_wait(loop->backend_fd,
                        events,
                        ARRAY_SIZE(events),
                        timeout);
      if (nfds == -1 && errno == ENOSYS) {
        uv__store_relaxed(&no_epoll_wait_cached, 1);
        no_epoll_wait = 1;
      }
    }

    // ...

    if (nfds == 0) { /* nfds == 0 { 本次没有事件, 如果超时, 则提前退出; 否则 continue; } */ }

    if (nfds == -1) { /* { 本次监听失败, a) 如果还没超时 continue; b) 如果被中断 EINTR continue;  } */ }

    have_signals = 0;
    nevents = 0;

    {
      /* Squelch a -Waddress-of-packed-member warning with gcc >= 9. */
      union {
        struct epoll_event* events;
        uv__io_t* watchers;
      } x;

      x.events = events;
      assert(loop->watchers != NULL);
      loop->watchers[loop->nwatchers] = x.watchers;
      loop->watchers[loop->nwatchers + 1] = (void*) (uintptr_t) nfds;
    }
    // 遍历 events
    for (i = 0; i < nfds; i++) {
      pe = events + i;
      fd = pe->data.fd;
      // 4.1 如果 events->fd 已经不在 loop->watchers[fd] 中 (可能是其他业务干掉的)
      //    则把 fd 从 loop->backend_fd 干掉 epoll_ctl(EPOLL_CTL_DEL)
      w = loop->watchers[fd];
      if (w == NULL) {
        epoll_ctl(loop->backend_fd, EPOLL_CTL_DEL, fd, pe);
        continue;
      }

      // 4.2 events->fd &= w->events, 即只关注上层 watcher 关注的事件 
      pe->events &= w->pevents | POLLERR | POLLHUP;
      // ...
      // 4.3 如果有事件 a) 信号事件 则 拉起标记 b) 其他事件 则 执行回调
      if (pe->events != 0) {
        if (w == &loop->signal_io_watcher) {  // 信号事件
          have_signals = 1;
        } else {
          uv__metrics_update_idle_time(loop);
          // 执行回调
          // 1. 如果是 tcp 服务器, 实际上是 uv__server_io, uv_tcp_listen 时传入
          // 2. 如果是 tcp 客户端, 实际上是 uv__stream_io, uv_tcp_init 时传入
          w->cb(loop, w, pe->events);
        }

        nevents++;
      }
    }
    // 5. 如果有信号事件, 之前遍历 events 被拉起标记
    //    则执行 信号回调 signal_io_watcher.cb
    if (have_signals != 0) {
      uv__metrics_update_idle_time(loop);
      loop->signal_io_watcher.cb(loop, &loop->signal_io_watcher, POLLIN);
    }

    // 6. 超时退出
    // 7. 时间更新
  }
}
```