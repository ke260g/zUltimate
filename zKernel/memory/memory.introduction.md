[TOC]
https://blog.csdn.net/qq_34827674/article/details/107042163
https://mp.weixin.qq.com/s/HJB_ATQFNqG82YBCRr97CA
# 1. 虚拟内存
1. 单片机的 CPU 是直接操作内存的"物理地址"
2. 为了并发运行多个进程; 需要把多个进程的地址空间进行隔离
3. 操作系统会提供一种机制，把不同进程的虚拟地址 和 不同内存的物理地址映射起来
    1. 不同进程的虚拟地址 可能存在冲突, 但是物理地址是独立
    2. 进程不允许直接访问物理地址
    3. 虚拟地址 与 物理地址 的映射, 对于进程而言是透明的
4. 两种地址的概念
    1. 虚拟内存地址 (vma): 程序使用的内存地址 (内核态也是使用虚拟地址的)
    2. 物理内存地址 (pma): 硬件内存的空间地址
5. 实现映射关系 需要硬件 MMU, 即可 ` cpu <--vma--> mmu <--pma--> memory`
    1. 提前在 MMU 设置 虚拟内存 到 物理内存 的映射关系表
    2. CPU 访问虚拟内存时, MMU 把虚拟内存 映射为 物理内存 访问 memory-device
    3. CPU 访问过程中是透明的
    4. 设置 MMU 发生在 scheduler 的 context-switch
6. 管理 vma 和 pma 的方法历史上有两种: 内存分段, 内存分页 (现在主要用这种)

# 2. 内存分段
## 2.1 分段机制
1. **虚拟地址** = **段选择因子** | **段内偏移量**
    1. **段选择因子** 在高位, 段内偏移量 在低位
    2. **段选择因子** = **段号** | 标记位
    3. **段号** 指示该地址在 **段表** 的位置
2. **段表** = **段内描述符** 的一维数组 (该表是放在内核空间中的, 同时也配置在MMU中)
    1. **段内描述符** = **段基地址**(物理内存的开始), **段界限**(物理内存的结束), 特权级 DPL
3. **物理内存** = **段基地址** + **段内偏移量**;
    2. 其中, **物理内存** 必须小于 **段界限**; 否则会有BUG
4. 如何分配物理内存是内存分配算法的责任, 此处暂不讨论
5. 伪代码
```c++
// 我自己瞎写的, 实际代码要 查看旧版本内核
typedef struct {
    uint32_t segment_offset: 20,
             segment_flag: 4,
             segment_id: 8; // little-endian, 高位存放在低地址
} vma_t;

typedef struct {
    uint32_t segment_base;
    uint32_t segnemt_edge;
    unit32_t dpl;
} segment_descriptor_t;

static segment_descriptor_t segment_table[0xff];

uint32_t vma_to_pma(uint32_t addr) {
    vma_t *vma = &addr;
    uint32_t segment_factor = vma->segment_id | vma->segment_flag;
    segment_descriptor_t *segment_descriptor = &segment_table[vma->segment_id];
    uint32_t pma = segment_descriptor->segment_base + vma->segment_offset;
    if (pma > segment_descriptor->segnemt_edge)
        return 0; // 越界
    return pma;
}
```

## 2.2 程序分段
1. 根据 elf 文件, 把进程拆分为 6 个segment, 从高往低
    1. kernel space
    2. stack
    3. library mapping
    4. heap            (这部分, 距离 .text 比较远)
    5. .data and .init (这部分, 距离 .text 比较近)
    6. .text
```log
562fb98c4000-562fb98cf000 r--p 00000000 103:04 43255017                  /usr/sbin/sshd
562fb98cf000-562fb994e000 r-xp 0000b000 103:04 43255017                  /usr/sbin/sshd
562fb994e000-562fb9996000 r--p 0008a000 103:04 43255017                  /usr/sbin/sshd
562fb9996000-562fb999a000 r--p 000d1000 103:04 43255017                  /usr/sbin/sshd
562fb999a000-562fb999b000 rw-p 000d5000 103:04 43255017                  /usr/sbin/sshd # 这部分是 .data, 在 elf 文件中有实际数据
562fb999b000-562fb99a0000 rw-p 00000000 00:00 0                          # 这部分是 .init, 在 elf 文件中有对应数据, 但占据进程空间
562fba53f000-562fba5fc000 rw-p 00000000 00:00 0                          [heap]
7f1b9bbea000-7f1b9bbef000 r--p 00000000 103:04 32508025                  /lib/x86_64-linux-gnu/libnss_systemd.so.2
7f1b9bbef000-7f1b9bc14000 r-xp 00005000 103:04 32508025                  /lib/x86_64-linux-gnu/libnss_systemd.so.2

```

## 2.3 内存碎片 (分段机制的弊端)
1. 物理内存碎片: 由于不连续小内存进程的销毁, 但无法被合并, 导致 大内存的进程 无法创建
2. 虚拟内存碎片: 由于进程空间内部, 堆中不连续小内存被释放, 导致 大内存无法被 malloc

## 2.4 内存交换
1. 物理内存碎片 的解决方法
    1. 当 大内存的进程 无法创建时; 找到不连续的进程若干
    2. 首先把 这些进程 放到 硬盘(swap 分区)
    3. 然后把 这些进程 的物理内存 重新映射为连续的一大块内存 (从而腾出空间)
    4. 最后把 这些进程 的物理内存 从磁盘中放回内存中
2. swap 的性能问题
    + 内存交换需要 disk-io, disk-io 速率很慢, 大概是内存的 10%
    + 频繁合并物理内存导致性能下降
    + 当然这种性能下降一般发生在进程创建 execl 时 (fork 有写时复制)
3. 虚拟内存碎片需要使用对象池解决

# 3. 内存分页 (解决 物理内存碎片, swap的性能问题)
1. 内存分段的弊端
    1. 内存分段能显式地整段映射, 产生连续的大内存,
    2. 但是会导致 物理内存碎片, 内存swap空间 太大的问题
2. 内存分页的策略 (如何解决 内存分段的弊端)
    1. 尽可能多地使用不连续的小内存 (减少物理内存碎片化导致的问题)
    2. 减少碎片化出现后, 内存swap空间的尺寸
3. 内存分页的设计
    1. 虚拟内存 和 物理内存 都切分为一块块固定大小的单元
    2. 该单元称为页, 大小是 4K
    3. **页表** = **页** 的一维数组
    4. **页表** 在运行过程中, 实实在在地配置到 MMU 中

## 3.1 分页机制
1. **虚拟地址** = **页表索引** | **页内偏移量**
    1. **页表索引** 在高位, **页内偏移量** 在低位
    2. **页表索引** 指示该地址在 **页表** 的位置
2. **页表** = **页** 的一维数组 (该表放在内核空间中, 同时也配置在MMU中)
    1. **页** = **虚拟页号**(可以在用页表位置 隐式表示) **物理页号**
3. **物理内存** = **物理页号** + **页内偏移量**
4. 页的伪代码 (自己瞎写的)
```c++
// 我自己瞎写的, 实际代码要 查看旧版本内核
typedef struct {
    uint32_t page_offset: 12, // 4K, 12-bits 表示
             page_table_index: 20; // little-endian, 高位存放在低地址
} vma_t;

typedef struct {
    // uint32_t vma_page_id; 这个成员实际上不需要, 我们把 页表索引 等同 "虚拟页号" 即可
    uint32_t pma_page_id;
} fake_page_t;

// 32位系统中, 页大小 4K, 最大寻址空间 4G, 因而最大有 1024*1024 个项
// 可能实际上是动态增长的, 这里瞎写
fake_page_t fake_page_table[1024*1024];

uint32_t vma_to_pma(uint32_t addr) {
    vma_t *vma = &addr;
    fake_page_t *fake_page = &fake_page_table[vma->page_table_index];
    uint32_t pma = fake_page->pma_page_id + vma->page_offset;
    return pma;
}
```
5. 缺页异常
    1. 当进程访问的虚拟地址在页表中查不到时
    2. MMU 会产生一个缺页异常, 传递到CPU
    3. 跳转到内核预先配置的 缺页回调
    4. 缺页回调中 分配物理内存(写入mmu), 更新进程页表, 最后再返回用户空间, 恢复进程的运行
6. 内存不足
    1. 把其他运行进程中, "最近没被使用"的页放到swap分区 (swapout)
    2. 腾出空间给当前运行的进程
    3. 当被 swapout 的页重新被引用时(缺页), 或进程调度, 再从 swap 分区放回内存( swapin )

## 3.2 分页优势 (为什么 分页能够避免 分段的问题)
1. 分配/释放 物理内存 以页为单位, 不会产生小于也就不会产生无法给进程使用的小内存
2. 内存不足产生的 swapout / swapin 只会交换少量的页, 不需要像 内存分段 那样交换一大个段
3. elf 延迟加载
    1. execl 先建立 虚拟内存和物理内存的页之间的映射 (必须先建表)
    2. 但  不会一次性都把 elf 文件加载到物理内存中
    3. 当进程读写对应虚拟内存页的数据/指令时，再加载到物理内存中

## 3.3 分页弊端
1. 维护页面的空间需要太大
2. 覆盖4G的内存页, 需要 1024*1024 个页表项, 假设页表项是 4byte, 即可需要4MB 空间存放
3. 如果200进程, 则需要 800M, 接近1G的空间, 相当吃内存

## 3.4 多级页表 (解决 分页弊端)
```c++
// 我自己瞎写的, 实际代码要 查看旧版本内核
typedef struct {
    uint32_t page_offset: 12, // 4K, 12-bits 表示
             l2_page_table_index: 10,
             l1_page_table_index: 10; // little-endian, 高位存放在低地址
} vma_t;

typedef struct {
    // uint32_t vma_page_id; 这个成员实际上不需要, 我们把 页表索引 等同 "虚拟页号" 即可
    uint32_t pma_page_id;
} fake_page_t;

typedef fake_page_t fake_l2_page_t[1024]; // 二级页表 一维数组
fake_l2_page_t *fake_l1_page_table[1024]; // 一级页表 是指向 二级页表的指针

uint32_t vma_to_pma(uint32_t addr) {
    vma_t *vma = &addr;
    fake_l2_page_t *fake_l2_page = fake_l1_page_table[vma->l1_page_table_index];
    fake_page_t * fake_page = fake_l2_page[vma->l2_page_table_index];
    uint32_t pma = fake_page->pma_page_id + vma->page_offset;
    return pma;
}
```
1. 把 1024*1024 的页表项, 拆分为2级页表
2. 完整地映射 4GB 地址空间就需要 4KB（一级页表 + 4MB(二级页表项) 的内存
3. 实际上一个进程不会分配4G的地址空间, 即"二级页表项"往往只有很少, 所以"二级页表项"按需分配
4. 一般地映射 进程地址空间只需要 4KB (一级页表) + 4KB * 若干二级页表项
    + 实际上一个进程甚至只有 512KB 页表内存
5. 多级分页 解决 一级分页的本质机制
    1. 任何分页机制都应该覆盖全地址空间, 否则进程无法正常工作
    2. 1级分页必须用 1024*1024 个表项进行覆盖, 这部分内存少不了的
    3. 2级分页中, Level1 使用1024 就已经覆盖了全地址空间, Level2 需要时再创建

## 3.5 64为系统的分页
全局页目录项 PGD (Page Global Directory)
上层页目录项 PUD (Page Upper Directory)
中间页目录项 PMD (Page Middle Director)
页表项 PTE (Page Table Entry)
偏移量 Offset

## 3.6 TLB (Translation Lookaside Buffer) (多级页表 高速缓存)
1. 多级页表优化了空间消耗, 但是转化物理地址 提高了 时间消耗
2. TLB 是一个嵌套在 cpu 中的 cache,
    + 存储最常访问的几个页表项; 提高访问速率
3. TLB 意译 页表缓存, 转址旁路缓存, 快表
    1. CPU 在寻址时，会先查 TLB, 如果没找到, 再查常规的页表
```txt
+----------- cpu  ----------+
| +-----+            +-----+|           +-----+
| | cpu | -- vma --> | mmu ||-- pma --> | mem |
| +-----+            +-----+|           +-----+
|                       v   |
|                    +-----+|
|                    | tlb ||
|                    +-----+|
+---------------------------+
```

# 4. 段页式内存管理
1. 把"内存分段"和"内存分页"合并起来, 即 段页式内存管理
    1. elf 文件按照 "内存分段" 的方法; 拆分为 segment ( .text, .data, .init, heap, mmap, stack, kernel)
    2. 每段再拆分为多个页 ( a) 段内的页是连续的; b) 段与段之间是不连续 )
1. 
# 5. linux 内存管理
## 5.1 x86 内存管理发展历史
1. 80286 使用的是 内存分段管理(硬件支持)
2. 80386 使用的是 页式内存管理(硬件支持)
3. 80386 的页式内存管理不完全, 
    1. 技术上, 在由段式内存管理所映射而成的地址上 再加上一层地址映射、
4. 80386 映射流程 ( 这个流程中, 进程运行时读写的地址是 logical-address )
    1. logical-address (段id+offset) -->  
    2. 段式内存管理unit(硬件) --> 
    3. linear-address (virtual-address) --> 
    4. 段式内存管理unit(硬件) -->
    5. physicall-address
    6. memory 内存条
## 5.2 linux 适配 80386
1. 背景:
    1. x86 硬件上, 先进行段式映射, 再进行页式映射
    2. linux 要实现完全的页式映射, 需要特殊的机制
2. 机制: (32 bit, 4G空间)
    1. 每段都是从 `0` ~ `4*1024*1024` 的整个虚拟空间
    2. 即所有的段的起始地址都是一样的
    3. 从而绕过 logical-address 
    4. 最终使得 进程/内核 使用的地址, 就是 linear-address
3. 32 bit 内核空间 和 用户空间
    1. 0xffffffff ~ 0xc0000000 内核空间 (1G)
    2. 0xc0000000 ~ 0x00000000 用户空间 (1G)
4. 64 bit 内核空间 和 用户空间
    1. 0xffffffffffffffff ~ 0xffff800000000000 (128T)
    2. 0x00007ffffffff000 ~ 0x0000000000000000 (128T)