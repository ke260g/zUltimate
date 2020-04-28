```c++
#include<asm/param.h>
# define HZ		CONFIG_HZ   // 写入到硬件, 定时硬中断. 
# define USER_HZ	HZ
# define CLOCKS_PER_SEC	HZ	/* frequency at which times() counts */

#include<linux/jiffies>
extern unsigned long volatile __jiffy_data jiffies;
```

`<linux/timex.h>` 中的实现, 依赖于 HZ 在 (12,1535) 范围, RFC-1589中定义.
所以, 使用 timex.h 的场景中不要把HZ调高于 1535

HZ       写入到硬件, 定时硬中断进入内核态
jiffies  记录HZ中断  进入内核态的次数, 该变量作为时间轴
 
比较函数 time_before / time_after / time_in_range
转换函数 
jiffies_to_clock_t/clock_t_to_jiffies
jiffies_to_msecs  / jiffies_to_usecs / msecs_to_jiffies / usecs_to_jiffies
timespec_to_jiffies / jiffies_to_timespec / timeval_to_jiffies / jiffies_to_timeval

获取当前时间 do_gettimeofday / current_kernel_time