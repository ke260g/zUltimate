
[TOC]
http://www2.rdrop.com/users/paulmck/rclock/ // 文档资源


# 9 RCU (Read-Copy-Update) (arch相关) (高性能锁)
http://www2.rdrop.com/users/paulmck/rclock/
1. 使用场景: 读多, 写非常非常少的场景
2. 写的流程:
    1. 先复制
    2. 写完之后等待所有读释放
    3. 把新的对象替换原有对象 
    4. 释放原有对象的资源

RCU 的优势:
1. 互斥锁, 仅允许单一线程访问数据
2. 读写锁, 当存在读线程时, 写线程会阻塞; 当存在写线程时, 读线程会阻塞. (已经优于 互斥锁)
3. RCU,    存在读线程时 写线程不阻塞; 存在写线程时, 读线程不阻塞.
    1. 加快 读写并发
    2. 加快多读 单写并发
4. 所以由于多写并发的特性, 在 频繁写and低频读场景中 RCU则不适用, 建议使用原始 rwlock
5. 为啥 RCU 多写并发要加锁 ?? 2020/08/12 ??, 我看内核代码都是这么用, 比如 dev.c

# 1. RCU 3个机制 (插入 / 删除 / 遍历)
## 1.1 insert (Publish-Subscribe Mechanism)
1. 发布: rcu_assign_pointer / list_add_rcu / hlist_add_head_rcu
2. 订阅: rcu_dereference / list_for_each_entry_rcu / hlist_for_each_entry_rcu
3. **订阅**机制 保证取得 **发布** 的新元素
## 1.2 delete (Wait For Pre-Existing RCU Readers to Complete)
+ 删除一个list中的元素; 需要等待其他读线程完成后; 才能释放, 否则会出core
1. 等待**读事件完成**的其他机制有: 读写锁, 事件订阅, 引用计数
2. 等待**读事件完成**在 rcu 中的实现方式优势在于: 无需 explicitly跟踪读记录
    + 即 没有死锁, 内存泄漏, 消耗性能 等风险
3. rcu **读事件** 称为 **RCU read-side critical sections**
    + 即 `rcu_read_lock()` 与 `rcu_read_unlock()` 包含的代码block
    + 因此 `rcu_read_lock()` 是
4. rcu 等待读事件完成 `synchronize_rcu()`

## 1.3 reader (Maintain Multiple Versions of Recently Updated Objects)

# 2 Question
## Q1: seqlock 也能做到单读单写并发; seqlock 和rcu有何区别? RCU 优势在何处?
1. seqlock 在读的过程中如果发生写并发; 则需要调用 `read_seqretry()`
2. `read_reqretry()` 返回 true 后读操作需要重新执行
3. 而 rcu  在读的过程中如果发生写并发; 则只需要继续执行读操作, 无感知
## Q2: What prevents the list_for_each_entry_rcu() from getting a segfault if it happens to execute at exactly the same time as the list_add_rcu()? 
1. linux 操作系统; 即时再多核环境中 读取指针面量 与 存储指针面量本身就是原子化的
2. 因此, 即使读写并发, read 和 list_add_rcu的每句指令 都是**有序分离**的
3. 而 list_add_rcu 指令中; 并没有把对象置空的操作
```c++
static inline void __list_add_rcu(struct list_head *new,
		struct list_head *prev, struct list_head *next)
{
	if (!__list_add_valid(new, prev, next))
		return;

	new->next = next;
	new->prev = prev;
    /* 把 prev->next = new; 执行发布机制 */
	rcu_assign_pointer(list_next_rcu(prev), new); // <--------------- 只有这两句才会有访问竞争
	next->prev = new;                             // <------*/        而这两句没有把对象置NULL
}
```
## Q3: hlist_for_each_entry_rcu() 和 list_for_each_entry_rcu() 的区别? (看源码比较)
## Q4: How would you modify the deletion example to permit more than two versions of the list to be active?

```c++
del() {
    spin_lock(); // 1. 多写并发要加锁, (读并发不需要)
    list_for_each_entry_rcu() // 2. 找到待删节点
        p = entry;
    if (p == NULL)
        spin_unlock();
    else {
        list_del_rcu();       // 3. rcu 删除; "订阅-发布"机制
        spin_unlock();        // 4. 解锁
        synchronize_rcu();    // 5. 等待 rcu_read_unlock() 引用完成
        kfree(p);             // 6. 释放对象
    }
}
```
1. 实际上就是多写并发在rcu上需要加锁
2. 假设, 不加锁(注释上述代码的 spin_lock()), 多个CPU可能同时找到一个对象p,
   则list_del_rcu()每个CPU都执行; 我看起来是没问题, 为啥要加锁?? <---- 2020/08/12
```c++
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	WRITE_ONCE(prev->next, next);
}
static inline void __list_del_entry(struct list_head *entry)
{
	if (!__list_del_entry_valid(entry))
		return;

	__list_del(entry->prev, entry->next);
}
```

## Q5: How many RCU versions of a given list can be active at any given time?
1. That depends on the synchronization design. 
2. If a semaphore protecting the update is held across the grace period, then there can be at most two versions, the old and the new.
3. However, if only the search, the update, and the list_replace_rcu() were protected by a lock, then there could be an arbitrary number of versions active, limited only by memory and by how many updates could be completed within a grace period.
4. But please note that data structures that are updated so frequently probably are not good candidates for RCU. That said, RCU can handle high update rates when necessary.

## Q6: How can RCU updaters possibly delay RCU readers, given that the rcu_read_lock() and rcu_read_unlock() primitives neither spin nor block?
1. 问题: 为什么 rcu_read_lock() 可以实现 spin_lock 的逻辑, 延迟读并发, 从而实现数据同步?
    + 这个问题实际上是讨论 RCU 的硬件实现本质
2. 答案: RCU 发布后, 其CPU 把 cache-lines 的数据 "无效化", 迫使其他CPU 遭遇"昂贵"的 cache-misses 
3. 原文作者的额外疑问: Can you design an algorithm that changes a data structure without inflicting expensive cache misses on concurrent readers? On subsequent readers?

# 3. 经典使用
## 3.1 rcu on read-most static array (用seqlock)
1. 这种场景rcu作者建议用 seqlock, 用rcu无法满足需求
2. 
## 3.2 rcu on read-most resize array

## 3.2 rcu on read-mostly list

# 更多背景
## ABA问题 及其本质
## COW (Copy-On-Write) 对比



## 9.3 例子
### 9.3.1 rcu多读单写
```c++
void get() {
    rcu_read_lock();
    list_for_each_entry_rcu() {
        if (found) _entry = entry;
    }
    rcu_read_unlock();
    return _entry;
}
void set() {
    list_add_rcu();
}
```
+ 这个场景比较特殊: 对象尺度是固定有限, 且全生命周期不销毁;

### 9.3.2 rcu多读多写
```c++
void get() {
    rcu_read_lock();
    list_for_each_entry_rcu() {
        if (found) _entry = entry;
    }
    rcu_read_unlock();
    return _entry;
}

void add() {
    spin_lock();
    list_for_each_entry() {

    }
    if (!found) list_add_rcu();
    spin_unlock();
}

void del() {
    spin_lock();
    list_for_each_entry() {
        if (found) {
            _entry = entry;
            list_del_rcu();
        }
        synchronize_rcu();
        free(_entry)
    }
    spin_unlock();
}
```