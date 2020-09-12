[TOC]
# overview
man 7 sysvipc
man 7 ftok
man 7 sem_overview
man 7 shm_overview
man 7 mq_overview
```c++
#include <sys/ipc.h>
key_t ftok(const char *pathname, int proj_id);  // 输入pathname; 输出 ipc 的 key (uuid)
```


# userspace utils
## ipcs  (show)
1. 显示 ipc 资源的 key_t, id(msqid / shmid / semid), 拥有者, 权限, 以及
    1. msq: 已占用的bytes 和 message(?)
    2. shm: bytes, attach计数, status(?)
    3. sem: 信号量-值
## ipcmk (create)
```sh
ipcmk -M $size # 创建共享内存  $size 指示 内存大小
ipcmk -Q       # 创建消息队列
ipcmk -S $nsem # 创建信号量    $nsum 指示 信号量值
ipcmk -p $perm # 指示权限, 如 777
```
## ipcrm (remove)
```c++
-M, --shmem-key shmkey
-m, --shmem-id shmid
-Q, --queue-key msgkey
-q, --queue-id msgid
-S, --semaphore-key semkey
-s, --semaphore-id semid
```
