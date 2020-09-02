[TOC]
# barrier 内存屏障
https://www.cnblogs.com/wuqi/articles/4475363.html  简单的介绍
http://ifeve.com/linux-memory-barriers              丰富的例子
Documentation/memory-barriers.txt                   蠢萌的官方


问题根源: cpu 流水线乱序 > 编译器 优化指令重排 > 硬件io问题 > SMP问题(两种 竞争 和 cache乱序)
解决方法: 4种屏障 = { 读屏障; 写屏障; 通用屏障; 数据依赖 }



## 先从 { x86 和 arm } 的 { smp_rmb smp_wmb smp_wb read_barrier_depends }  分析
```c++
// include/linux/compiler-gcc.h
#define barrier() __asm__ __volatile__("": : :"memory")
```

###  arm
```c++
// arch/arm
#ifndef CONFIG_SMP
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#else
#define smp_mb()	dmb(ish)
#define smp_rmb()	smp_mb()
#define smp_wmb()	dmb(ishst)
#endif
```

### // arch/x86
```c++
#ifdef CONFIG_SMP
#define smp_mb()	mb()
#ifdef CONFIG_X86_PPRO_FENCE
# define smp_rmb()	rmb()
#else
# define smp_rmb()	barrier()
#endif
#define smp_wmb()	barrier()

#else /* !SMP */
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif /* SMP */

#ifdef CONFIG_X86_32
#define mb() alternative("lock; addl $0,0(%%esp)", "mfence", X86_FEATURE_XMM2)
#define rmb() alternative("lock; addl $0,0(%%esp)", "lfence", X86_FEATURE_XMM2)
#define wmb() alternative("lock; addl $0,0(%%esp)", "sfence", X86_FEATURE_XMM)
#else
#define mb() 	asm volatile("mfence":::"memory")
#define rmb()	asm volatile("lfence":::"memory")
#define wmb()	asm volatile("sfence" ::: "memory")
#endif


```

内存屏障的实现 默认交由编译器 `barrier(); ` 调用汇编函数 ?memory?

单核cpu中; 内存屏障统一使用编译器的 `barrier()`
x86 和 arm 分别针对 r/w 屏障有各自的优化?
内存屏障有 6个方法  mb() rmb() wmb() smp_mb() smp_rmb() smp_wmb()


barrier 解决的问题: 单处理器下的CPU流水线导致的乱序问题  编译器重排
                    多处理器下的内存同步等问题。


## 问题根源
1. CPU 流水线导致的指令乱序             (硬件保证 保证上下文因果关系)
2. 编译器针对CPU 的流水线进行指令重排    (软件保证 保证上下文因果关系)
```c++
// 优化前; 
// "a %= 2" 为耗时操作; 需要等待 "a++;" 完成
// 虽然 cpu 会有乱序; 但是 编译器级会先进行一次优化
a++;
a %= 2; // 耗时操作
b++;
c++;

// 优化后; "a++;" "b++;" "
a++;
b++;
c++;
a %= 2;
```
4. 没有 上下文因果关系; 但是必须保证时序的场景 (即 不可以 指令重拍/ 指令乱序)
```c++
// 场景1: 这两条指令 表示读取 引脚5 的电平状态; 必须保证时序性
*addr = 5;     // 引脚号   指示寄存器
  val = *data; // 引脚电平 指示寄存器

// 场景2: 多cpu-情况下
// cpu-a)
obj->data = get_data(); // 准备好数据
obj->ready = true;      // 使能cpu-b) 执行数据
// cpu-b)
if (obj->ready)
    run_task(obj->data);
```
5. 加上屏障
```c++
// 场景1:
*addr = 5;     //
  mb();        // <---- 干掉编译器的  "指令重排"
  val = *data; //

// 场景2:
// cpu-a)
obj->data = get_data();
wmb();              // wmb() 用以解决问题2.1 
obj->ready = true;

// cpu-b)
if (obj->ready) {
    rmb();          // rmb() 用以解决问题2.1
    run_task(obj->data);
}
    
// 场景2的问题有3个:
// 问题2.1: cpu-a) 和 cpu-b) 各自cpu-中的访问 data 和 ready 需要保证时序性 (硬件访问时)
// 问题2.2: cpu-b) 还有一个问题;  data / ready 从内存中读取上的 cache 中的顺序是 不确定的
//          也就是说; 即使保证 `if (obj-ready)` 先于 `do_task(obj-data)` 的时序,
//                   不能保证 `obj->ready 在 cpu-b) 中的
//          当然这个问题只会发生于并行 cache 的架构, 如 alpha; 用 smp_* 解决
//          实际上, 某些架构的 smp_wmb/smp_rmb/smp_mb 是等同于无 smp_* 接口的
// 问题2.3: cpu-a) 和 cpu-b) 如果是纯软件访问时; 需要保证 b) 比 a) 后; 即加锁同步
//          当然, 软件访问时, 单个cpu-中的 data ready 不需要保证时序性
//          因此, 内核代码中可以用内存屏障, 处理单读单写的并发.
//          但是, 用户态没有内存屏障, 所以用户态必须加锁.

// 后记: (联系 kfifo的 in/out 实现逻辑; 感觉说不大通, 为啥只有 smp_wmb 而没有 smp_rmb)

// 场景3:
/// cpu-a)            
init(newval); // 写操作
wmb();
data = &newval;
// cpu-b)
p = data;
read_barrier_depends();
val = *p;

这里的屏障就可以保证：如果data指向了newval，那么newval一定是初始化过的。
```



内存屏障主要有：读屏障、写屏障、通用屏障、优化屏障、几种。

读屏障: 用于限制读操作 屏障之前的读操作一定会先于屏障之后的读操作完成，写操作不受影响，同属于屏障的某一侧的读操作也不受影响。
写屏障: 用于限制写操作 而通用屏障则对读写操作都有作用。
而优化屏障则用于限制编译器的指令重排，不区分读写。
