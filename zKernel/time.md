内核全局变量 xtime jiffies
xtime:   开机时只会从rtc硬件时钟中获取一次
jiffies: 记录开机后, 内核中断的次数; 用来计算uptime

内核时钟概念:
## HZ    每秒产出内核中断的次数
cat /proc/interrupts | grep timer
make menuconfig: Processor type and features ---> Timer frequency (250 HZ) 
## Tick  HZ的倒数; 每?秒发生一次中断
## Jiffies 开机后 内核中断的次数

## clocksource_register_hz 时钟注册函数

# high freq
1. 放大内核CPU软件系数
2. 关闭软狗
3. 放大 jiffies 每次中断后, 增加的数值, 如增加100