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