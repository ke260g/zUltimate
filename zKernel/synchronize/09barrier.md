[TOC]
# barrier 内存屏障
https://www.cnblogs.com/wuqi/articles/4475363.html  简单的介绍
http://ifeve.com/linux-memory-barriers              丰富的例子(一定要看懂这个)
Documentation/memory-barriers.txt                   蠢萌的官方

## 问题根源
cpu 流水线乱序 > 编译器 优化指令重排 > 硬件io问题 > SMP问题(两种 竞争 和 cache乱序)

## 解决方法: 内存屏障 x 5
1. rmb 读屏障:   用于限制读操作 屏障之前的读操作一定会先于屏障之后的读操作完成，写操作不受影响，同属于屏障的某一侧的读操作也不受影响。
2. wmb 写屏障:   用于限制写操作
3.  mb 通用屏障: 不区分读写
4. smp_read_barrier_depends(): 如果屏障后的读操作以来屏障前的; 则保证屏障前的 cache读取先于屏障后的.
    + 仅仅作用于 cache; 与 memory  无关
    + 只有 并行 cache 的cpu 架构才这种问题
5. barrier 优化屏障: 限制编译器优化导致的指令重排


# x86 内存屏障
## barrier 优化屏障 gcc
```c++
// include/linux/compiler-gcc.h
#define barrier() __asm__ __volatile__("": : :"memory")
```

## 单核方法
```c++
#ifdef CONFIG_X86_32
#define mb() alternative("lock; addl $0,0(%%esp)", "mfence", X86_FEATURE_XMM2)
#define rmb() alternative("lock; addl $0,0(%%esp)", "lfence", X86_FEATURE_XMM2)
#define wmb() alternative("lock; addl $0,0(%%esp)", "sfence", X86_FEATURE_XMM)
#else
#define mb() 	asm volatile("mfence":::"memory")
#define rmb()	asm volatile("lfence":::"memory")
#define wmb()	asm volatile("sfence" ::: "memory")
#define read_barrier_depends()	do { } while (0)
```

## smp 方法
```c++
#ifdef CONFIG_SMP
#define smp_mb()	mb()
#define smp_wmb()	barrier()
#define smp_read_barrier_depends()	read_barrier_depends() // x86 没有平行cache

#ifdef CONFIG_X86_PPRO_FENCE
# define smp_rmb()	rmb()
#else
# define smp_rmb()	barrier()
#endif

#else /* !SMP */

#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#define smp_read_barrier_depends()	do { } while (0) // x86 没有并行cache

#endif /* SMP */
```
1. 这些方法用于仅仅避免多核时序问题 （代码逻辑 保证单核没问题)
2. 但在单核CPU 中; 不存在多核竞争; 
3. 所以只需要优化屏障 barrier 即可

# 问题根源
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

// 场景3:
// cpu-b 实际上只于 cpu-a) 共享一个数据 data
// cpu-b 不存在硬件读写时序 的问题
// 实际上; cpu-b 需要保证的是 data 是经过初始化的;
// 所以;   只需要 read_barrier_depends 保证 cache 的更新时序即可
```

