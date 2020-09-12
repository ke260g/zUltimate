[TOC]
shmat 的本质是, 构造一个新的page, page 物理地址指向 共享内存的地址
# 接口 <sys/mman.h>  (-lrt)
```c++
// system v 老式接口
int shmget(key_t key, size_t size, int shmflg);          // 创建匿名shm; 返回 shmid;
int shmctl(int shmid, int cmd, struct shmid_ds *buf);    // 删除匿名shm;
void *shmat(int shmid, const void *shmaddr, int shmflg); // 映射到用户空间
int shmdt(const void *shmaddr); // 反映射

// linux futex 接口
int shm_open(const char *name, int oflag, mode_t mode); // add / get 命名shm_fd 的方法
int shm_unlink(const char *name); // del 方法; 只del当前进程; 内核中的对象知道没有进程引用后才销毁
```
1. `shmctl(cmd = IPC_RMID)` 删除shm;
    + 最后的销毁逻辑, 需要等所有 shmat的进程 都执行 shmdt 后才会执行
2. `shm_unlink()` 形同 `shmctl(cmd = IPC_RMID)`

# 使用
```c++
int main(int argc, char *argv[]) {
    void *buff = NULL;
    int buff_size = 4096;
    const char *name = "message_center";
    int shm_fd = shm_open(shm_path, O_RDWR | O_CREAT, 544);  // 1. 调用 open(/dev/shm/message_center);
    if (shm_fd < 0) goto error;
    ftruncate(shm, buff_size);                               // 2. 形同 文件扩容
    // mmap MAP_PRIVATE 仅线程共享; MAP_SHARED 进程间共享     // 3. 映射到进程空间
    buff = (void *)mmap(NULL, buff_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    munmap(shm, buff_size);
    shm_unlink(shm_path); // 调用 unlink("/dev/shm/" "name") // 5. 没有unlink /dev/shm/message_center将一直存在
    return 0;
}
```