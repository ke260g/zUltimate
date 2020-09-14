[TOC]
# 接口1(unix) <sys/sem.h>
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

# 接口2(futex) <semaphore.h> (必须 -lphread)
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
    2. != 0, 信号量可以在进程间共享, 因此 `@sem` 对象也应该通过 `(shm_open + mmap)/shmget` 创建
4. `sem_open()` 命名信号量可以在进程间共享
    1. `@oflag |= O_CREAT;` 用于创建信号量
    2. 如果 `O_OREAT` 且 信号量原来不存在, 则首次创建, 此时必须提供 mode 和 value, 否则返回失败
    3. `@oflag |= O_CREAT | O_EXCL;` 如果信号量已经存在, 则返回失败 (形同 open)
5. `sem_close()` 和 `sem_unlink()` 的区别
    1. `sem_close()` 只是本进程资源的释放, 内核态 sem 的对象直到所有open进程都 close 了才释放
    2. `sem_unlink()` 仅删除命名信号量的命名绑定(即 `sem_open()` 不能获取同一个, 除非 `O_CREAT`)
    3. `sem_unlink()` 删除命名绑定时, 不会马上销毁内核态sem, 已open的进程仍然可以使用

# 使用
```c++
int main(int argc, char *argv[]) {
    const char *sem_name = "sem_ctx";
    printf("open\n");
    int nsem = 1;
    sem_t * sem = sem_open(sem_name, O_CREAT, 0544, nsem); // 最终 open(/dev/shm/sem_ctx)
    if (!sem) goto error;
    printf("wait\n");
    sem_wait(sem);
    printf("close\n");
    sem_destroy(sem);
    sem_unlink(sem_name);
error:
    return 0;
}

```
```log
write(1, "open\n", 5)                   = 5
statfs("/dev/shm/", {f_type=TMPFS_MAGIC, f_bsize=4096, f_blocks=8235053, f_bfree=8235053, f_bavail=8235053, f_files=8235053, f_ffree=8235052, f_fsid={val=[0, 0]}, f_namelen=255, f_frsize=4096, f_flags=ST_VALID|ST_NOSUID|ST_NODEV}) = 0
futex(0x7f862ce67390, FUTEX_WAKE_PRIVATE, 2147483647) = 0
openat(AT_FDCWD, "/dev/shm/sem_ctx", O_RDWR|O_NOFOLLOW) = -1 ENOENT (没有那个文件或目录)
getpid()                                = 3780671
lstat("/dev/shm/xMF2lP", 0x7ffc298fcd50) = -1 ENOENT (没有那个文件或目录)
openat(AT_FDCWD, "/dev/shm/xMF2lP", O_RDWR|O_CREAT|O_EXCL, 0544) = 3
write(3, "\1\0\0\0\0\0\0\0\200\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 32) = 32
mmap(NULL, 32, PROT_READ|PROT_WRITE, MAP_SHARED, 3, 0) = 0x7f862ceae000
link("/dev/shm/xMF2lP", "/dev/shm/sem_ctx") = 0
fstat(3, {st_mode=S_IFREG|0544, st_size=32, ...}) = 0
unlink("/dev/shm/xMF2lP")               = 0
close(3)                                = 0

write(1, "wait\n", 5)                   = 5
write(1, "close\n", 6)                  = 6
unlink("/dev/shm/sem_ctx")          = 0

1. 先检查 /dev/shm 这个是 shm/sem futex实现共用的, 不存在则用 futex 系统调用创建
2. 再检查 /dev/shm/sem_ctx, 不存在;
3. 先用创建随机文件名shm_fd, 进行mmap, 然后把shm作为 信号量的本质底层句柄; 存放 信号量值
4. 准备好信号量句柄后; link 再 unlink
```

# 实现
```c++
sem_wait(sem_t *sem) {
while (true) {
   if (atomic_decrement_if_positive(sem->count))
       break;
   futex(&sem->count, FUTEX_WAIT, 0, ...);
}

sem_post(sem_t *sem) {
   n = atomic_increment(sem->count);
   // Pass the new value of sem->count
   futex(&sem->count, FUTEX_WAKE, n+1);
}
```