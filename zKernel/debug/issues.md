### Q: ko 版本魔数 对不上
1. 获取目标机子的 /proc/version
2. 改源码 linux-4.14.76/include/config/kernel.release
3. 改源码 linux-4.14.76/include/generated/utsrelease.h
4. 编模块
5. grep 一下模块里面是不是真的有

mount -t debugfs nodev /sys/kernel/debug/
mount -t proc none /proc/

运行时内核函数地址 https://blog.csdn.net/gatieme/article/details/78310036





