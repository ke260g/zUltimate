local_irq_disable() 	禁止本地中断传递
local_irq_enable() 	    激活本地中断传递
local_irq_save() 	    保存本地中断传递的当前状态，然后禁止本地中断传递
local_irq_restore() 	恢复本地中断传递到给定的状态
disable_irq() 	        禁止给定中断线，并确保该函数返回之前在该中断线上没有处理程序在运行
disable_irq_nosync() 	禁止给定中断线
enable_irq() 	        激活给定中断线
irqs_disabled() 	    如果本地中断传递被禁止，则返回非0；否则返回0
in_interrupt() 	        如果在中断上下文中，则返回非0；如果在进程上下文中，则返回0
in_irq() 	            如果当前正在执行中断处理程序，则返回非0；否则返回0


什么是本地中断??

resource = kzalloc(sizeof(*resource) + 1, irqs_disabled() ? GFP_ATOMIC: GFP_KERNEL);

已知一个.c文件, 一个.h文件, 如何知道 .c文件是如何引用到 .h文件的? 引用依赖栈如何??


#define in_irq()        (hardirq_count())
#define in_softirq()    (softirq_count())
#define in_interrupt()  (irq_count())     // 软件中和硬中断

irqs_disabled() 大概率是同于 in_irq(), 即硬件处于硬中断中

硬件进入中断上下文, 驱动设计者大概率会把irq_disable掉, 因而  irqs_disabled() == true

所以一般 `{ if (in_irq() || irqs_disabled()) return is_in_hard_irq; }` 表示处于硬中断上下文中
