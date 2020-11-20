[TOC]
1. 这个三个都是每次遍历都会执行
2. 执行顺序 idl prepare io-big-poll check
3. idle, prepare, check 底层都是 loop-watcher 实现
4. loop-watcher.c 使用 macro `#define` 了 5个方法
    + init / start / stop / run
```c++
#define UV_LOOP_WATCHER_DEFINE(name, type)

UV_LOOP_WATCHER_DEFINE(prepare, PREPARE)
UV_LOOP_WATCHER_DEFINE(check, CHECK)
UV_LOOP_WATCHER_DEFINE(idle, IDLE)
```
# 1. 初始化 uv_idle_init uv_prepare_init uv_check_init
```c++
  // 1. uv__handle_t 多态初始化
  // 2. 赋值回调函数
  int uv_##name##_init(uv_loop_t* loop, uv_##name##_t* handle) {
    uv__handle_init(loop, (uv_handle_t*)handle, UV_##type);     
    handle->name##_cb = NULL;                                   
    return 0;                                                   
  }                                                             
```
# 2. 开启 uv_idle_start uv_prepare_start uv_check_start
```c++
  // 1. 如果已经开启; 则返回
  // 2. 无效参数检查
  // 3. 入列 loop->idle_handles / loop->prepare_handles / loop->check_handles
  // 4. 赋值回调函数
  // 5. 标记开启
  int uv_##name##_start(uv_##name##_t* handle, uv_##name##_cb cb) {  
    if (uv__is_active(handle)) return 0;                             
    if (cb == NULL) return UV_EINVAL;                                
    QUEUE_INSERT_HEAD(&handle->loop->name##_handles, &handle->queue);
    handle->name##_cb = cb;                                          
    uv__handle_start(handle);                                        
    return 0;                                                        
  }          
```
# 3. 停止 uv_idle_stop uv_prepare_stop uv_check_stop
```c++
  // 1. 如果已经停止; 则返回
  // 2. 出列 loop->idle_handles / loop->prepare_handles / loop->check_handles
  // 3. 标记停止
  int uv_##name##_stop(uv_##name##_t* handle) {                               \
    if (!uv__is_active(handle)) return 0;                                     \
    QUEUE_REMOVE(&handle->queue);                                             \
    uv__handle_stop(handle);                                                  \
    return 0;                                                                 \
  }     
```

# 4. 遍历
```c++
  // 1. 把队列放到临时队列中 loop->idle_handles / loop->prepare_handles / loop->check_handles
  // 2. 遍历临时队列 queue (从头开始遍历)
  // 3. 先把节点移出 queue
  // 4. 再把节点加回原队列中 loop->idle_handles / loop->prepare_handles / loop->check_handles
  // 5. 执行节点回调
  void uv__run_##name(uv_loop_t* loop) {          
    uv_##name##_t* h;                             
    QUEUE queue;                                  
    QUEUE* q;                                     
    QUEUE_MOVE(&loop->name##_handles, &queue);
    while (!QUEUE_EMPTY(&queue)) {                
      q = QUEUE_HEAD(&queue);                     
      h = QUEUE_DATA(q, uv_##name##_t, queue);
      QUEUE_REMOVE(q);                            
      QUEUE_INSERT_TAIL(&loop->name##_handles, q);
      h->name##_cb(h);                            
    }                                             
  }                                               
```