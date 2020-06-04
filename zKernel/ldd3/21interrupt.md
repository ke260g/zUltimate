[TOC]
# 中断
+ 该文只有一些简单的使用; 而不是中断的底层实现
+ 设备的中断号通常不容易定义; 此时可以把中断号作为参数; 有insmod传入
    + 运行时的中断号通过查询 其他硬件的信息获得; 如PCI总线
## 方法
```c++
// kernel/irq/manage.c
typedef irqreturn_t (*irq_handler_t)(int, void *);

// 注册/销毁
/**
 * @param irq    中断号; CPU硬件相关
 * @param name   对应 /proc/interrupts
 * @param dev_id 传递给中断回调的上下文; 可以为NULL; 一般为设备对象
 */
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
	    const char *name, void *dev_id);
void free_irq(unsigned int irq, void *dev_id);

// 注册/销毁 每个CPU
int request_percpu_irq(unsigned int irq, irq_handler_t handler,
		   const char *devname, void __percpu *percpu_dev_id);
void free_percpu_irq(unsigned int irq, void __percpu *dev_id);

// 使能 单个中断
void disable_irq_nosync(int irq);
void disable_irq(unsigned int irq);
void disable_percpu_irq(unsigned int irq);
void enable_irq(unsigned int irq);
void enable_percpu_irq(unsigned int irq, unsigned int type);

// 使能 当前CPU的中断
void local_irq_disable(void);
void local_irq_enable(void);
void local_irq_save(unsigned long flags);
void local_irq_restore(unsigned long flags);
```

### 中断注册 flags
IRQF_SHARED
IRQF_PROBE_SHARED
IRQF_TIMER          (__IRQF_TIMER | IRQF_NO_SUSPEND | IRQF_NO_THREAD)
IRQF_PERCPU
IRQF_NOBALANCING
IRQF_IRQPOLL        只能用于shared中断; 以提高性能
IRQF_ONESHOT        没触发一次中断后; 销毁中断回调
IRQF_NO_SUSPEND     suspend(睡眠)中; 中断不停止; 即允许唤醒
IRQF_FORCE_RESUME   睡眠后启动时; 强制开启中断
IRQF_NO_THREAD      该中断无法 `线程化`?
IRQF_EARLY_RESUME   睡眠后启动时; 在syscore流程中开启中断; 而不是硬件恢复阶段开启

## 调试
1. `cat /proc/interrupts`
    + 中断ID, 每个CPU中断次数, ?中断信息? 中断名称(注册方法::传入的name参数)
2. `cat /proc/stat | grep intr`
    + 第一列数字表示总的 中断次数; 
    + 第二列数字表示中断ID为 0 的中断次数
    + 第N 列数字表示中断ID为 N - 2 的中断次数

## share interrupt
+ 用于 pci bus, universal-serial-bus 等下接各种各样的硬件
+ 注册时 flag 带上 IRQF_SHARED, 使得同一个 中断ID 被多个硬件共享