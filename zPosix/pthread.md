## pthread_create
```c++
clone(child_stack=0x7fb0966e8fb0, flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, parent_tidptr=0x7fb0966e99d0, tls=0x7fb0966e9700, child_tidptr=0x7fb0966e99d0);
```
1. pthread_create(); 使用了 clone 调用; 共享了 "VM / FS / FILES / SIGHAND"  等资源
    + 但是每个线程有其独立的 信号队列 以及 多线程共享的信号队列

## pthread_mutex
```c++
int pthread_mutex_lock (pthread_mutex_t *__mutex) {
    atomic_dec(mutex.value);
    if (mutex.value != 0)
        futex(&mutex.value, FUTEX_WAIT, ...)
    else
        return 0;
}
int pthread_mutex_unlock (pthread_mutex_t *__mutex) {
    if (mutex.value != 1)
        futex(&mutex.value, FUTEX_WAKE, ...)
    else
        return 0;
```

# 线程退出
## 1. 方法1: pthread_cancel pthread_testcancel pthread_join
1. pthread_cancel 是异步; 而且要在
   带有"线程取消点"的系统调用中, 才会退出
2. 所以, 为了更好地保证退出
   协线程插装 `pthread_testcancel();`
   然后主线程 `pthread_join()`;
## 2. 方法2: socketpair() 二次挥手
1. 两个线程都使用 epoll 机制, 
   初始化线程时创建 `socketpair(AF_UNIX, SOCK_STREAM, 0, pairfd[2]);`
   pairfd 要设置为 nonblock
   ```c++
   int flags = fcntl(pairfd[0], F_GETFL, 0);
   fcntl(pairfd[0], F_SETFL, flags|O_NONBLOCK);

   int flags = fcntl(pairfd[1], F_GETFL, 0);
   fcntl(pairfd[1], F_SETFL, flags|O_NONBLOCK);
   ```
2. 主线程当需要取消 协线程时, 往 `fd = pairfd[0];` 发消息
   主线程开始等待 `fd = pairfd[0];`消息, 加入 epoll, POLLIN 事件
3. 协线程一直等待 `fd = pairfd[1];`消息, 加入 epoll, POLLOUT, 
   有消息则退出, 协线程销毁资源完成后, 往 `fd = pairfd[1];` 发消息
4. 主线程等待 `fd = pairfd[0];`有消息, 表明协线程已经完成退出了,
   销毁协线程相关的上下文资源
> 实际中: socketpair() 会丢包吗? 
> 参考 af_unix 丢包原因一般是接收缓存满了
> 因此 pairfd[2] 保证只用于退出信息