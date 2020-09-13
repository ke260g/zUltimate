
# 8 seqlock 只用于一个integer (arch相关) (高性能锁)
1. 使用场景: 写比较罕见但很快; 但是读比较频繁的场景;
2. 使用逻辑: 非阻塞性读; 读完之后处理; 处理后检查是否有写更新;
3. 使用范式
```c++
 // Expected non-blocking reader usage:
do {
    seq = read_seqbegin(&value);
    //    ...
} while (read_seqretry(&value, seq));
```
4. 使用例子 kernel/time/jiffies.c
```c++
u64 get_jiffies_64(void)
{
    unsigned long seq;
    u64 ret;

    do {
        seq = read_seqbegin(&jiffies_lock);
        ret = jiffies_64;
    } while (read_seqretry(&jiffies_lock, seq));
    return ret;
}
```

## 8.1 方法 (初始化 / r占用 / r释放 / w占用 / w释放)
```c++
#define seqlock_init(x) // 初始化

// r占用
unsigned read_seqbegin(const seqlock_t *sl);
unsigned int read_seqbegin_irqsave(seqlock_t *lock, unsigned long flags);
// r释放
int read_seqretry_irqrestore(seqlock_t *lock, unsigned int seq, unsigned long flags);
// r占用后检查是否需要重来
unsigned read_seqretry(const seqlock_t *sl, unsigned start);

// w占用
void write_seqlock(seqlock_t *lock);
void write_seqlock_irqsave(seqlock_t *lock, unsigned long flags);
void write_seqlock_irq(seqlock_t *lock);
void write_seqlock_bh(seqlock_t *lock);

// w释放
void write_sequnlock(seqlock_t *lock);
void write_sequnlock_irqrestore(seqlock_t *lock, unsigned long flags);
void write_sequnlock_irq(seqlock_t *lock);
void write_sequnlock_bh(seqlock_t *lock);
```
## 8.2 实现