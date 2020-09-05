[TOC]
# <sys/sem.h>
man 7 sem_overview
```c++
#include <sys/sem.h>
int semget(key_t key, int nsems, int semflg);    // obtain or create
int semctl(int semid, int semnum, int cmd, ...); // set/get/del  on a sem
int semop(int semid, struct sembuf *sops, size_t nsops); // decr/incr/wait on a sem
int semtimedop(int semid, struct sembuf *sops, size_t nsops, const struct timespec *timeout);

struct sembuf { // 用于 semop
    unsigned short sem_num;  /* semaphore number */     //
    short          sem_op;   /* semaphore operation */  // <-- 包含操作方法
    short          sem_flg;  /* operation flags */      // <-- IPC_NOWAIT 和 SEM_UNDO
};

union semun {   // 用于 semctl 第4个参数
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */  // 每个成员; 对应着 sem_num
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

struct semid_ds {
    struct ipc_perm sem_perm;  /* Ownership and permissions */
    time_t          sem_otime; /* Last semop time */
    time_t          sem_ctime; /* Last change time */
    unsigned long   sem_nsems; /* No. of semaphores in set */
};
```
1. `semget()` 获取 `@semid`
2. 一个 `@semid` 有很多个 `@sem_num`

4. `@sem_op`
    1. `@sem_op > 0` 往信号量加 `sem_op`
    2. `@sem_op < 0` 尝试往信号量减 `sem_op`的绝对值; 如果 `信号量 + sem_op < 0`; 则阻塞直到大于等于0后才执行
    3. `@sem_op = 0` 阻塞直到信号量 等于0, 大于和小于都不行
5. `@sem_flg |= IPC_NOWAIT;` 指示非阻塞
6. `@sem_flg |= SEM_UNDO;` ??

# <semaphore.h> (必须 -lphread)
```c++
int sem_post(sem_t *sem); // incr on a sem
int sem_wait(sem_t *sem); // decr on a sem
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

int sem_getvalue(sem_t *sem, int *sval); // current value of the semaphore

int sem_init(sem_t *sem, int pshared, unsigned int value); // init unnamed semaphore
int sem_destroy(sem_t *sem); // destroys the unnamed semaphore

sem_t *sem_open(const char *name, int oflag);
sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
int sem_unlink(const char *name); // removes the named semaphore referred to by name.
int sem_close(sem_t *sem); // closes the named semaphore referred to by sem
```
1. `sem_wait()` 把信号量减1; 如果减去后信号量为负, 则阻塞直到信号量被异步加为1
2. `sem_destroy()` 和 `sem_init()` 配对使用, 即匿名信号量
3. `sem_init()` 的 `@value`
    1. == 0, 信号量只能在线程间共享, 即全局变量
    2. != 0, 信号量可以在进程间共享, 因此 `@sem` 对象也应该通过 `shm_open/mmap/shmget` 创建
4. `sem_open()` 命名信号量可以在进程间共享
    1. `@oflag |= O_CREAT;` 用于创建信号量
    2. 如果 `O_OREAT` 且 信号量原来不存在, 则首次创建, 此时必须提供 mode 和 value, 否则返回失败
    3. `@oflag |= O_CREAT | O_EXCL;` 如果信号量已经存在, 则返回失败 (形同 open)
5. `sem_close()` 和 `sem_unlink()` 的区别
    1. `sem_close()` 只是本进程资源的释放, 内核态 sem 的对象直到所有open进程都 close 了才释放
    2. `sem_unlink()` 仅删除命名信号量的命名绑定(即 `sem_open()` 不能获取同一个, 除非 `O_CREAT`)
    3. `sem_unlink()` 删除命名绑定时, 不会马上销毁内核态sem, 已open的进程仍然可以使用

# 