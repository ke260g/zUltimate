[TOC]
# <sys/mman.h>  (-lrt)
```c++
int shm_open(const char *name, int oflag, mode_t mode); // add / get 方法
int shm_unlink(const char *name); // del 方法; 只del当前进程; 内核中的对象知道没有进程引用后才销毁
```