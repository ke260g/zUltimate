[TOC]
计算机储存器分为 register cache memory disk network, 其中访问 disk, network 较慢.
而针对 disk 的访问的优化分为:  zero_copy, direct_io, asynchrous_io
以减少访问磁盘的次数(即可 单次访问更多的数据), 从而提高系统的吞吐量

# 1. 无 DMA 的磁盘访问流程
1. **syscall** read() and trap
2. **cpu** send request to **disk**
    + 然后进程被调度走
3. **disk** prepare data into **disk-buffer**
    + 这个时候其它进程是可以运行的, 但不能再次访问disk, 必须加锁
4. **disk** raise interrupt
5. **disk-interrupt-handler** copy **disk-buffer** into **memory** (so called **page-chache**)
    + 这期间cpu需要把大量数据从 disk 拷贝到 memory
6. **kernel-page-cache** copy_to_user(), and read() return data

# 2. 带 DMA 的磁盘访问
1. **syscall** read() and trop
2. **cpu** send request to **dma-controller**
    + 然后进程被调度走
3. **dma-controller** send request to **disk**
    + 这个时候其它进程是可以运行的, 但不能再次访问disk, 必须加锁
4. **disk** notify **dma-controller**
5. **dma-controller** copy **disk-buffer** into **memory**
    + 这个过程不需要 cpu 参与
6. **dma-controller** raise interrupt
7. **dma-interrupt-handler** obtain
8. copy_to_user(), and read() return data

# 3. 从 disk 到 network 传输流程分析 
## 3.1 read(disk) > write(socket) 流程
1. userspace read trap      (context switch 1)
2. disk to memory (dma )    (copy 1)
3. kernelspace to userspace (copy 2)   // fs缓冲区
4. syscall read return      (context switch 2)
5. userspace write trap     (context switch 3)
6. userspace to kernelspace (copy 3)   // socket 缓冲区
7. memory to netcard (dma)  (copy 4)
8. syscall write return     (context switchd 4)
 
## 3.2 sendfile 流程 (网卡不支持 Scatter-Gather Direct Memory Access)
1. userspace sendfile trap      (context switch 1)
2. disk to memory (dma)         (copy 1)
3. kernelspace to kernelspace   (copy 2) // page-cache > socket缓冲区
4. memory to netcard (dma)      (copy 3)
5. syscall sendfile return      (context switchd 2)

## 3.3 sendfile 流程 (网卡支持 Scatter-Gather Direct Memory Access)
1. userspace sendfile trap      (context switch 1)
2. disk to memory (dma)         (copy 1)
4. memory to netcard (dma)      (copy 2) // page-cache 直接到 网卡dma
    1. 支持 SG-DMA 的网卡可以直接从 page-cache 拷贝到 DMA
5. syscall sendfile return      (context switchd 2)

## 3.4 分析 与 优化
1. 一次 从 disk 到 network 的需要经历 4次拷贝, 4次切换
2. 无论是否使用了 DMA; disk / network 上到用户态分别需要2次拷贝
    2. device <---> memory(kernelspace)
    3. kernelspace <---> userspace
3. 使用 dma 减少了 cpu 参与 device 与 memory 间的拷贝             (优化a: 减少cpu参与拷贝)
    + dma 是每个硬件上自己的组件, 布线时直连内存条
4. 实际上 userspace 与 kernelspace 间的拷贝是多余的, 因为这个过程据没有改变
5. 可以用 mmap 替代掉 userspace 与 kernelspace 间的拷贝 (优化b: 减少拷贝次数)
    + 从而实现 2次拷贝, 4次切换
6. `sendfile(int out_fd, int in_fd, off_t *offset, size_t count);` (优化c: 减少切换和拷贝)
    + 3次拷贝, 2次切换
    + in_fd 指示 file, out_fd 指示 socket
    + 三次拷贝: disk > memory, kernelspace > kernelspace, kernelspace > netcard
7. sendfile + sg-dma, 实现   (优化4: 减少切换和拷贝)
    + 2次拷贝, 2次切换
    + 两次拷贝: disk > memory > netcard
    + 从而实现零拷贝, 即该过程中 cpu 不参与内存拷贝
8. 异步io, 减少cpu 的阻塞等待 (优化5: 减少cpu阻塞)

## 3.5 优化总结
1. 使用 dma 减少了 cpu 参与 device 与 memory 间的拷贝             (优化a: 减少cpu参与拷贝)
2. 可以用 mmap 替代掉 userspace 与 kernelspace 间的拷贝 (优化b: 减少拷贝次数)
    + 2次拷贝, 4次切换
3. sendfile 无 SG-DMA (优化c: 减少切换和拷贝)
    + 3次拷贝, 2次切换
4. sendfile + sg-dma  (优化4: 减少切换和拷贝)
    + 2次拷贝, 2次切换

# 4. page-cache 内核空间中磁盘数据的缓存
1. 目的: 减少磁盘的访问, 从而提高性能
2. 策略:
    1. 缓存最近被访问的数据, 当page-cache不足时销毁最久未被重新访问的缓存 
    2. 预读功能, 加入userspace只请求16K, 但实际上, 内核把后续的16K, 即共32K一次性请求上来
3. 理论: (局部性原理)
    1. 被访问的数据在短时间内 大概率再次被访问
    2. 被访问的数据的附近数据在短时间内 大概率被访问
4. 限制: (大文件问题)
    1. 大文件无法使用 DMA 导致性能下降
    2. 文件太大，可能某些部分的文件数据被再次访问的概率比较低
    3. PageCache 长时间被大文件占据, 而需要频繁访问的小文件 则无处可放
    4. PageCache 没有为大文件发挥作用

# 5. 内核 I/O 调度器
1. 内核 I/O调用器 建立在 PageCache 的基础上
2. a)调度算法 缓存尽可能多的 I/O 请求在 PageCache 中; 合并更大的IO请求到 DMA
3. b)调度算法 是实现 PageCache 预读功能的主体
4. 从而减少 disk 的访问

# 5. 大文件传输 ( aio + directio )
1. aio: 避免大文件长时间阻塞的问题
2. directio: 绕过 PageCache 避免影响 PageCache 在其他小文件的应用
3. directio 适用场景
    1. userspace 实现了磁盘缓存: 虽然没有预读功能, 但仍然可以关闭 PageCache 节省性能
    2. 传输大文件: 大文件使用 PageCache 会严重消耗性能
## 5.1 mysql   关闭 PageCache 的方法
## 5.3 nginx   关闭 PageCache 的方法
```conf
location / {
    sendfile on; // 小于 512m, 使用 sendfile, zero.copy
    aio on;
    directio 512m; // 超过这个尺寸的文件, 将使用 aio + directio
}
```
## 5.2 syscall 关闭 PageCache 的方法