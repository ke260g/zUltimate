[TOC]
# 1. 进程管理
## 1.1 进程本质
1. 运行状态的 elf: 代码段segment + 其他资源(fd / signal / env等 / 内存页 / 数据段segment)
2. 进程是 资源管理单元; 线程才是执行单元; 内核调度的对象本质是线程;
    + 实际上 在调度器的实现看来 线程是一种特殊的进程
3. 进程本质是两种虚拟: 虚拟处理器 + 虚拟内存
    1. 虚拟处理器让进程"以为"独占一个; 但实际上处理器是多进程共享
    2. 虚拟内存  让进程"以为"拥有整个系统的内存
## 1.2 进程描述 (struct task_struct)
1. 内核中所有进程共享一个队列; 用 `task_struct->tasks` 双向链表串联; rcu同步
    1. `#define next_task(p)` 用以遍历
2. 当前执行进程用 current() 方法获取；该方法的实现因架构而异; x86 中的实现
    1. 该方法返回 percpu 变量 `current_task`
    2. 变量 `current_task` 在进程调度函数 `__switch_to()`
3. 进程描述符是 pid_t; 实际是int类型; 最大值 32768()
    1. 改写 `/proc/sys/kernel/pid_max` 可调整
4. init 进程的描述是静态定义的全局符号 `init/init_task.c` `EXPORT_SYMBOL(init_task);`
    1. `for_each_process()` 就是从 init_task 开始遍历(`next_task()`)整个系统的进程
5. 进程父子关系
```c++
// 父进程
current->parent;
current->real_parent;
// 子进程
struct task_struct *task;
struct list_head   *list;
lsit_for_each(list, &current->children) {
    struct task_struct *task = list_entry(list, struct task_struct, sibling);
    // task 即子进程
} // task_struct->children 作为链表头; 其链表节点指向 子进程的 task_struct->sibling
```
 
### 1.2.1 进程状态 (有且只有以下5种之一; 对应 man ps 的 `STAT` 列)
```c++
#define TASK_RUNNING		 0 // (ps::STAT::R) 进程a)可执行但未执行 or b)正在执行[用户态中必然是该状态]
#define TASK_INTERRUPTIBLE	 1 // (ps::STAT::S) 阻塞在系统调用中; 可接受信号被唤醒
#define TASK_UNINTERRUPTIBLE 2 // (ps::STAT::D) 阻塞在系统调用中; 无法被唤醒；一般是硬件IO
#define __TASK_STOPPED		 4 // (ps::STAT::T) 进程暂停(sh::Ctrl-z);
#define __TASK_TRACED		 8 // (ps::STAT::t) ptrace 系统调用跟踪的

#define set_current_state(state_value)   // smp中 必须使用内存屏障设置 set_mb
#define set_task_state(tsk, state_value) // smp中 必须使用内存屏障设置 set_mb
```
1.  `TASK_RUNNING`   的进程也会出现内核态; 比如调度到用户态前就是 `a) 可执行但未执行)`
2. `__TASK_STOPPED`  进入该状态的信号有 SIGSTOP / SIGTTOU / SIGTTIN / SIGTSTP；退出该状态 SIGCONT
3. 状态机

## 1.3 进程创建 (vfork or clone)
1. libc库中 fork  封装了系统调用 `clone()`
2. libc库中 vfork 实际上有相应的系统调用 `vfork()`
3. clone() 直接调用 `do_fork()` 传参
4. `do_fork()` 主要调用了 `copy_process()` 完成进程创建
    + `{ fork / vfork / clone } -> do_fork -> copy_process`

### 1.3.1 写时拷贝 ( Copy-On-Write )
1. 传统的 fork 调用会把旧进程**所有的内存数据**拷贝到新进程; 然后在 exec 时再写一次
2. 内核的 fork 调用 使用**延迟拷贝**策略: fork后子进程与父进程共享相同的内存页?
    + 子进程权限只读 (怎么实现的?)
    + 共享的是 `struct page` 指向的内存页; 但 `struct page` 需要新构造
3. 只有在 exec 调用 时; 才会复制父进程的数据 (即写时拷贝)
4. fork() 的实际开销是 复制父进程的页表项(造表) 和 初始化子进程的 task_struct
    + 免除了不必要的 页表数据 复制

### 1.3.2 copy_process() 关键业务
1. `dup_task_struct()` 创建新的 `task_struct` + `thread_info` + 内核栈 + 进程栈
    1. 创建新的 `task_struct`: `alloc_task_struct_node()` 
    2. 创建新的 `thread_info`: `alloc_thread_info_node()` 
    3. 内核栈: `kaiser_map_thread_stack()`
    4. 进程栈: `setup_thread_stack()`
2. 检查进程数是否超出最大值
    + `if (atomic_read(&p->real_cred->user->processes) >= task_rlimit(p, RLIMIT_NPROC))`
3. 进程 `task_struct->flags` 设置
    1. `p->flags &= ~(PF_SUPERPRIV | PF_WQ_WORKER);` 取消超级用户标记
	2. `p->flags |= PF_FORKNOEXEC;` 表明进程 fork 但还没 exec
3. 清除不继承的资源 (如 pending信号 / 定时器 / 一系列统计信息等 )
4. 初始化进入调度器 `sched_fork()`; 其中设置为 `TASK_RUNNING`
5. **写时拷贝** or **只读共享** 一系列的进程资源
    + `copy_semundo` `copy_files` `copy_fs` `copy_sighand` `copy_signal`
    + `copy_mm` `copy_namespaces` `copy_io` `copy_thread`
6. 分配 pid `alloc_pid()`

### 1.3.3 vfork
1. vfork() 不拷贝父进程的页表项; 子进程作为独立线程运行在 父进程地址空间
2. 父进程vfork() 后阻塞; 直到子进程 `exit()` or `exec()`
#### vfork 实现详解
1. vfork 系统调用 传参`CLONE_VFORK` 给 do_fork()
2. 父进程 `copy_process()` 初始化子进程的 completion 对象 `task_struct->vfork_done = NULL`
3. 父进程 `do_fork()` 在 `copy_process()` 后；通过等待 `struct completion` 实现阻塞
4. 子进程 `mm_release()` 在进程退出时 `complete_vfork_done(tsk)` 解除父进程的阻塞
```c++
do_fork() { // 父进程软中断
    copy_process();
    struct completion vfork; p->vfork_done = &vfork; init_completion(&vfork); // 加锁
    wake_up_new_task(p);     // 唤醒: 子进程
    wait_for_vfork_done(p, &vfork); // 阻塞 (wait_for_completion_killable(vfork))
    put_pid(pid);            // 使父进程重新调度
}
mm_release() { // 子进程软中断 exec() or exit()
    if (tsk->vfork_done)
		complete_vfork_done(tsk); // 解锁: 使得父进程继续运行
}
```

#### vfork 比起 fork的优势
1. 不复制父进程的页表项 `task_struct->mm` (如果有 COW-页表项 机制; 则该优点不存在)
2. 不复制父进程的页数据 (在2.2内核才有此优势；因为当时fork没有 COW-页数据 的设计)

## 1.4 线程实现
1. linux 内核中没有线程的技术概念 (windows / solaris 就有)
2. 线程本质 就是 mm_struct；只是与主线程共享大量资源
3. 线程调度 完全同于进程; 没有特殊的线程调度算法
### 1.4.1 线程创建 (pthread_create => clone => do_fork)
1. 线程创建系统调用就是 `clone()` strace 可以观察到
2. 线程创建  `do_fork()` flags 为: `CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD` 等
    + 从而; 子进程与父进程共享 地址空间(VM) 文件系统资源(FS) fd表(FILES) 信号回调(SIGHAND) 线程组(CLONE_THREAD)
    ```c++
    copy_process() {
        // 复制 一份当前进程的 task_struct
        // 底层 arch_dup_task_struct() 直接 *tsk = *current;
        p = dup_task_struct(current);

        // 新的 pid
        p->pid = pid_nr(pid);

        // 如果是线程, CLONE_THREAD, 继承 tgid
        // 否则是进程, tgid 就是 pid
        if (clone_flags & CLONE_THREAD) { 
            // ...
            p->tgid = current->tgid; // 继承
        } else {                         
            // ...
            p->tgid = p->pid;        // 自己
        }
        // 如果是线程, CLONE_FILES, 增加 current->files 的引用计数
        // 否则 dup_fd(current->files);
        copy_files(clone_flags, p);
        
        // 如果是线程, CLONE_FS，增加 current->fs 的引用计数;
        // 否则 copy_fs_struct(current->fs);
        copy_fs(clone_flags, p);
        
        // 如果是线程, CLONE_VM, 增加 current->mm 的引用计数;
        // 否则 dup_mm(stk);
        copy_mm(clone_flags, p);
        
        // 如果是线程, CLONE_SIGHAND, 增加 current->sighand 的引用计数
        // 否则 kmem_cache_alloc 重新分配一份
        copy_sighand(clone_flags, p);
    }
    ```
3. 对比:  进程创建 `do_fork()` flags 为: `CLONE_SIGCHLD`
4. 对比: vfork创建 `do_fork()` flags 为: `CLONE_VFORK | CLONE_VM | SIGCHLD`

### 1.4.2 内核线程 (create_kthread => kernel_thread => do_fork)
1. 内核线程的本质也是一个进程`task_struct`
    + 但没有地址空间映射 `task_struct->mm = NULL;`; 因为不用换页
2. 通过 `do_fork` 创建 flags 为 ` CLONE_VM | CLONE_FS | CLONE_FILES | SIGCHLD `
3. 通过 `do_exit` 自行退出
4. 通过 `kthread_stop()` 停止
5. 通过 `kthread_run()`  开始; 实际上就是 `wake_up_process()`的封装

## 1.5 进程终结
### 1.5.1 进程的销毁 do_exit
```c++
do_exit() {
    exit_signals(tsk);//  主要是 `task_struct->flags |= PF_EXITING` 和清除其他信号
    acct_update_integrals(tsk); // ?记账功能?
    tsk->exit_code = code;      // 退出码; 调用者传入
    exit_mm(tsk);     // 释放 `task_struct->mm` (不是立刻销毁; 因为fork 页表共享)
    exit_sem(tsk);    // 离开 信号量的等待队列
    exit_files(tsk);  // dec fd的引用;           (引用数为zero才销毁)
    exit_fs(tsk);     // dec 文件系统族源的引用;（引用数为zero才销毁)
    cgroup_exit(tsk); // 
    exit_notify(tsk, group_dead); // 向父进程发SIGCHILD; 本进程 exit_state = EXIT_ZOMBIE;
    tsk->state = TASK_DEAD;
    schedule();       // 调度到别的进程
}
```
1. 进程处于 `EXIT_ZOMBIE` 不会再次被调度; 
    + 需要手动 schedule(); 否则只能cpu不再调度其他进程; 永远阻塞在 do_exit 中
### 1.5.2 进程描述符的销毁 release_task
1. 为了使得父进程可以获取子进程的信息; 子进程的进程描述符并非在do_exit() 中销毁
    + 通过 `wait4()` 获取
2. 实际上需要父进程在 `release_task()` 中销毁 `task_struct`
    + 如果父进程没有显式 `wait4()`; 则往上传递直到 init进程
```c++
void release_task(struct task_struct * p) {
    /* 1. __unhash_process() -> detach_pid() -> 删除pidhash 中的该进程
     * 2. 释放进程的剩余其他资源 (sigqueue / sighand)
     */
    __exit_signal(p);

    // delayed_put_task_struct -> put_task_struct -> __put_task_struct
    call_rcu(&p->rcu, delayed_put_task_struct);
}
```

### 1.5.3 孤儿进程的寻父逻辑
1. 如果父进程提前退出; 则子进程的 `task_struct` 如何销毁?
2. 因此; 父进程退出时; 在线程组内找一个线程作为 养父进程(real_parent); 没找到则往上**寻父**
3. 父进程 `do_exit()` -> `exit_notify()` -> `forget_original_parent()` 实现
```c++
static struct task_struct *find_new_reaper(struct task_struct *father) {
    struct pid_namespace *pid_ns = task_active_pid_ns(father);
    struct task_struct *thread;

    thread = father;
    // 逻辑1: 在线程组中找; 找到则返回;
    while_each_thread(father, thread) {
        if (thread->flags & PF_EXITING)
            continue;
        if (unlikely(pid_ns->child_reaper == father))
            pid_ns->child_reaper = thread;
        return thread;
    }
    // 逻辑2: 找不到; 往上**寻父** 直到init
    for (reaper = father->real_parent; reaper != &init_task; reaper = reaper->real_parent) {
    }
}

static void forget_original_parent(struct task_struct *father) {
    reaper = find_new_reaper(father); // 为小孩们 寻找养父
    list_for_each_entry_safe(p, n, &father->children, sibling) {
        struct task_struct *t = p;
        do {
            t->real_parent = reaper; // 把所有子进程的 `task_struct->real_parent` 重新设置
            if (t->parent == father) {
                BUG_ON(t->ptrace);
                t->parent = t->real_parent;
            }
        } while_each_thread(p, t);
        reparent_leader(father, p, &dead_children);
    }
}
```