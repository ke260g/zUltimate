[TOC]
# 1. 对象定义 typedef struct uv_timer_s uv_timer_t
先不看了, 没啥特别的

# 2. 初始化 uv_timer_init
```c++
int uv_timer_init(uv_loop_t* loop, uv_timer_t* handle) {
  // 1. uv_handle 多态的初始化
  // 2. 初始化参数
  uv__handle_init(loop, (uv_handle_t*)handle, UV_TIMER);
  handle->timer_cb = NULL;
  handle->timeout = 0;
  handle->repeat = 0;
  return 0;
}
```
# 3. 拉起定时器 uv_timer_start
```c++
int uv_timer_start(uv_timer_t* handle,
                   uv_timer_cb cb,
                   uint64_t timeout,
                   uint64_t repeat) {
  uint64_t clamped_timeout;

  // 1. 无效参数检查
  // 2. 如果已经开启, 则先stop掉(移出 小顶堆)
  // 3. 根据当前时间, 计算超时时间差 clamped_timeout
  // 4. 其他数据的初始化赋值
  // 5. 插入小顶堆
  // 6. 标记开启
  if (uv__is_closing(handle) || cb == NULL)
    return UV_EINVAL;

  if (uv__is_active(handle))
    uv_timer_stop(handle);

  clamped_timeout = handle->loop->time + timeout;
  if (clamped_timeout < timeout)
    clamped_timeout = (uint64_t) -1;

  handle->timer_cb = cb;
  handle->timeout = clamped_timeout;
  handle->repeat = repeat;
  /* start_id is the second index to be compared in timer_less_than() */
  handle->start_id = handle->loop->timer_counter++;

  heap_insert(timer_heap(handle->loop),
              (struct heap_node*) &handle->heap_node,
              timer_less_than);
  uv__handle_start(handle);

  return 0;
}
```

# 4. 关掉定时器 uv_timer_stop
```c++
int uv_timer_stop(uv_timer_t* handle) {
  // 1. 没激活提前返回
  // 2. 移处小顶堆 (timer_less_than 是传入到小顶堆的比较函数)
  // 3. 标记停止
  if (!uv__is_active(handle))
    return 0;
  heap_remove(timer_heap(handle->loop),
              (struct heap_node*) &handle->heap_node,
              timer_less_than);
  uv__handle_stop(handle); // 最终在 uv_process_endgames 中销毁
  return 0;
}
```
# 5. 重启定时器 uv_timer_again
```c++
int uv_timer_again(uv_timer_t* handle) {
  // 1. null 参数检查
  // 2. handle->repeat 为零直接返回
  // 3. 先停止定时器 (实际上在 uv__run_timers 中该逻辑多余)
  // 4. 重启定时器
  if (handle->timer_cb == NULL)
    return UV_EINVAL;

  if (handle->repeat) {
    uv_timer_stop(handle);
    uv_timer_start(handle, handle->timer_cb, handle->repeat, handle->repeat);
  }

  return 0;
}
```
# 5. 遍历定时器 uv__run_timers 
```c++
void uv__run_timers(uv_loop_t* loop) {
  struct heap_node* heap_node;
  uv_timer_t* handle;

  for (;;) {
    // loop->timer_heap 本身就是维护好的小顶堆(增删时维护)
    // 1. 从 定时器 小顶堆 取得最近生效的定时器
    // 2. 使用 container_of 获取定时器句柄
    // 3. 如果 还没超时则终止循环; 由于小顶堆机制, 后续节点也没超时
    // 4. 停止定时器
    // 5. 重启定时器 根据 timer->repeat 比较执行; 0则直接返回
    // 6. 执行定时器回调
    heap_node = heap_min(timer_heap(loop));
    if (heap_node == NULL)
      break;

    handle = container_of(heap_node, uv_timer_t, heap_node);
    if (handle->timeout > loop->time)
      break;

    uv_timer_stop(handle);
    uv_timer_again(handle);
    handle->timer_cb(handle);
  }
}
```