[TOC]
# 1. 背景知识
1. 中断值称为 IRQ 线; 有些是动态分配 有些是静态定义
    + 例如: (cat /proc/interrupts)
    + x86 IRQ 0 表示定时器中断
2. IRQ: 中断请求; 每个中断对应一个唯一的 中断值(硬件上对应 连接CPU 的线)
3. ISR: interrupt service routine；中断服务程序 (即 回调)
4. 异常: (x86 TRAP)
    1. 异常不同于一般的中断; 异常属于同步中断; 即 异常的产生 必须与 处理器时钟同步
    2. 异常的产生是指 CPU 执行到"异常"的指令: 如 除以0 缺页(跟进这个) 软件主动触发
        必须通知操作系统处理; 否则就会硬件重启

## 1.1 中断上半部TH 和 中断下半部BH
1. 中断回调的本质矛盾在于 需要处理业务 又快又多
2. 快: 如果慢了; 硬件中断就会丢掉; 导致硬件异常; 网卡芯片的开发经验
3. 多: 硬件业务本来就是要有很多东西需要处理的!!!
4. 因此把中断回调拆分为两个部分:
    1. 上半部 TopHalf    = 因为此时关掉其他中断; 处理严格时限的业务(设置寄存器); 越快越好
    2. 下半部 BottomHalf = 其他硬件没有时限的业务; 推迟到下半部处理

## 1.2 { 中断禁止 vs 加锁 vs 抢占禁止 } => 最终目的 都是同步
1. 抢占禁止 = 方式其他进程调度 的并发访问
2. 中断禁止 = 防止其他中断回调 的并发访问 (通常伴随 禁止内核抢占)
3. 加锁     = 防止其他处理器   的并发访问 (通常伴随 禁止本地处理器的中断)
4. 无论是 抢占禁止 还是 中断禁止 都没有防止其他处理器的并发
5. 因此多处理器架构(smp) 中的并发访问都需要通过加锁实现同步

# 2. irq 使用
1. CPU 实现硬件中断入口; 把中断号传到内核;
    + 最终调用框架的 irq_to_desc 和 generic_handle_irq 调用注册的回调 (或自己实现)
    + 硬件中断入口 还需要 irq_enter() irq_exit() 处理上下文
2. 其他驱动则 `request_irq()` 和 `free_irq()` 即可
## 2.1 irq 使用方法 (注册 / 销毁)（标记flags)
```c++
/**
 * @irq:  中断号 /proc/interrupts 首列; 必须与硬件相关
 * @handler: 中断回调函数
 * @flags: 标记; 有特殊指示
 * @name: 中断名 /proc/interrupts 末列; 指示中断含义
 * @dev: free_irq 指示内核共享中断的 具体中断; 即 @dev <-> @handler 对应; 
 *       共享中断 注册和销毁必须非NULL；非共享可以为 NULL
 */
int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags, const char *name, void *dev);
void free_irq(unsigned int irq, void *dev);
/**
 * @return IRQ_NONE 表示产生中断的设备 并非 回调理想的产生源; // 历史遗留；后面不再用了
 *         IRQ_HANDLED 回调正确处理了中断
 */
typedef irqreturn_t (*irq_handler_t)(int irq, void *dev);

// flags
#define IRQF_DISABLED		0x00000020  // 处理该中断时; 内核把其他中断给关掉; 很少用
#define IRQF_SHARED		0x00000080      // 多个中断回调 共享同一个 中断号
#define IRQF_PROBE_SHARED	0x00000100  
#define __IRQF_TIMER		0x00000200
#define IRQF_PERCPU		0x00000400
#define IRQF_NOBALANCING	0x00000800
#define IRQF_IRQPOLL		0x00001000
#define IRQF_ONESHOT		0x00002000
#define IRQF_NO_SUSPEND		0x00004000
#define IRQF_FORCE_RESUME	0x00008000
#define IRQF_NO_THREAD		0x00010000
#define IRQF_EARLY_RESUME	0x00020000
#define IRQF_TIMER		(__IRQF_TIMER | IRQF_NO_SUSPEND | IRQF_NO_THREAD)
```
## 2.2 irq 共享中断
1. 一般来说共享中断 的实现需要满足3个规则
    1. `request_irq()` 的 `@flags` `IRQF_SHARED`
    2. `request_irq()` 的 `@dev`   必须唯一
    3. 中断回调中必须 于硬件交互 判断是否时 "逻辑上" 绑定的设备产生了中断
## 2.3 irq_disable() 延迟关闭
1. disable_irq() 对于 edge-interrupts 会出现中断丢失
2. 为了决绝这种问题；disable_irq() 并不会把硬件上的 中断掩码给关掉
3. 硬件上的中断掩码只有 在 flow handler 中进入中断回调时才关掉
4. disable_irq 设置了 IRQ_DISABLED 标记
5. 当中断触发时 且 IRQ_DISABLED 标记存在; 则设置 IRQ_PENDING
6. 当中断开始时 且 IRQ_PENDING  标记存在；则进入 中断回调
## 2.3 irq 架构实现 (x86)
同一个中断不可重入原则; 内核已经把 cpu 中断关掉了; 必要时还需要关掉设备的中断;
### 2.3.1 硬件 cpu 中断入口
x86 的内核中断实现:
    entry.S 汇编码 定义cpu 中断的统一回调;
    在回调中 把 中断号 传参给 do_IRQ 函数
### 2.3.2 中断实现 do_IRQ
1. 进入 硬中断上下文 `irq_enter()` // 看不懂这是做什么? 退出软中断吗??
2. `exit_idle()` 看不懂
3. 从传入参数的寄存器中; 读取中断号
4. `handle_irq()` 根据中断号; 找到对应的 `ISR`; 即 `irq_to_desc()`
    + 如果有 调用完ISR后 返回 true; 找不到 则返回 false
5. 退出中断上下翻文 `irq_exit()` // 看不懂这是做什么?

# 3. irq 框架 (arch 无关)
1. 提供通用的 API  用以 request / enable / disable / free 中断
2. 提高 drivers 在不同中断类型的 cpu 架构中代码的移植性
3. 优化每种类型中断的 处理流程 flow  (避免针对 不同的类型 写很多 if-else)
    1. 旧版本 __do_IRQ() super-handler 统一所有中断类型; 优化时不同类型中断分支较多
4. 逻辑拆分: 'irq flow' and 'chip details' （中断流程 和 芯片细节)
5. 架构分层:
    1. High-level driver API
    2. High-level IRQ flow handlers
    3. Chip-level hardware encapsulation
## 3.1 关键数据结构
```c++
struct irq_desc {
    interrupt flow method    // 即 desc->handle_irq()
    interrupt chip structure // 即 desc->irq_data.chip
}
```
## 3.2 驱动层使用接口 ( high-level irq control api )
由驱动代码 控制中断时调用
```c++
request_irq()        // 注册中断回调
free_irq()           // 销毁中断回调
local_irq_disable()  // 禁止本地cpu中断; x86 asm 指令 cli (clear interrupt)
local_irq_save()     // 保存中断状态; 在函数的复杂调用中；保证同一函数的局部上下文 中断状态一致
local_irq_enable()   // 使能本地cpu中断; x86 asm 指令 sti (set interrupt)
local_irq_restore()  // 恢复中断状态
disable_irq()        // 禁止单个中断；该函数是全局的(所有处理器) + 同步的(等待所有处理器的 该中断的回调返回后才返回)
enable_irq()         // 使能单个中断
disable_irq_nosync() // 禁止单个中断；异步的(不等待 中断回调 马上返回)  SMP 架构中才有用 ?什么场景用到?
synchronize_irq()    // 等待单个中断回调返回后才返回;                   SMP 架构中才有用 ?什么场景用到?
irqs_disabled()      // 返回 true; 指示本地中断被禁止; 否则 返回 false
in_interrupt()       // 返回 true; 指示在中断上下文;   否则 返回 false
in_irq()             // 返回 true; 指示在中断回调中；  返回 返回 false
in_softirq()         // 返回 true; 指示在软中断上下文; 否则 返回 false
in_serving_softirq() // 返回 true；指示在软中断回调中；否则 返回 false
irq_set_irq_type()
irq_set_irq_wake()
irq_set_chip_data()
```

## 3.3 irq类型相关的流程方法 irq flow handlers
 ( simple / level / fasteoi / edge / edge_eoi / nested / percpu / percpu_devid )
```c++
handle_level_irq()
handle_edge_irq()
handle_fasteoi_irq()
handle_simple_irq()
handle_percpu_irq()
handle_edge_eoi_irq()
handle_bad_irq()
```

## 3.4 芯片中断的相关method (struct irq_chip)
arch  相关的中断方法; 即 dess->irq_data.chip 的接口实现
由 流程方法调用
```c++
irq_ack()
irq_mask_ack() // optional
irq_mask()
irq_unmask()  
irq_eoi()       // optional; required for EOI flow handlers
irq_retrigger() // optional;
irq_set_type()  // optional;
irq_set_wake()  // optional;
```

## 3.5 helper function
### irq_to_desc() 根据中断号找到注册的中断desc
### irq_enter() irq_exit() 中断上下文进入/退出
### generic_handle_irq 根据中断desc 执行中断回调