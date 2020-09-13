# 4 completion
1. sem 进行了性能优化 专门处理 (大部分时间是 可行, 少部分时间是 不行)
    1. 优化点1: likely
    2. 优化点2: 流程
2. 但是 存在在大部分 时间是不行的场景 比如初始化
+ `down()` 开始调用了 `likely()`
+ `wait_for_completion()` 没有使用 `likely()`

## 4.1 方法 ( 创建 /  等待 / 通知 )
```c++
void init_completion(struct completion *x);

// 等待 ( 阻塞 / 非阻塞 / 超时 / 阻塞可唤醒 )
void wait_for_completion(struct completion *); // 阻塞
bool try_wait_for_completion(struct completion *x); // 非阻塞
unsigned long wait_for_completion_timeout(struct completion *x,unsigned long timeout);
int wait_for_completion_interruptible(struct completion *x); // 阻塞但可唤醒 一般用这个
int wait_for_completion_killable(struct completion *x);

// IO等待
void wait_for_completion_io(struct completion *x); // 不可被唤醒, 通常是IO等待

// 通知
void complete(struct completion *c);
void complete_all(struct completion *c);

// 检查有没有 等待者
bool completion_done(struct completion *x);
```

## 4.2 实现

