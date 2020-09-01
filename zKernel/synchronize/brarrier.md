[TOC]
# barrier 内存屏障


## 先从 { x86 和 arm } 的 { smp_rmb smp_wmb smp_wb }  分析
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
