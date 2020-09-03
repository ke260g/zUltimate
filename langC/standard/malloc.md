[TOC]
入门介绍 https://blog.csdn.net/wang13342322203/article/details/80862382
需要联系内核中; 内存的相关实现


malloc分配的内存大小至少为size参数所指定的字节数
malloc的返回值是一个指针，指向一段可用内存的起始地址
多次调用malloc所分配的地址不能有重叠部分，除非某次malloc所分配的地址被释放掉
malloc应该尽快完成内存分配并返回（不能使用NP-hard的内存分配算法）
实现malloc时应同时实现内存大小调整和内存释放函数（realloc和free）

malloc和free函数是配对的，如果申请后不释放就是内存泄露;
如果无故释放那就是什么都没有做，释放只能释放一次.
如果释放两次及两次以上会出现错误（但是释放空指针例外，释放空指针其实也等于什么都没有做，所以，释放多少次都是可以的）


new 与 malloc bf

普遍采用虚拟内存地址技术:
当涉及内存地址时， 都是使用虚拟内存地址;
每个进程仿佛自己独享一片 4G 的内存 (32位架构)

虚拟地址空间的作用 = 简化程序的编译  和 方便操作系统对进程间内存的隔离管理
实际上一个进程需要用到的内存时比较少的; 只是取值空间跨度非常大而已


机器语言层面都是采用虚拟地址，当实际的机器码程序涉及到内存操作时，
需要根据当前进程运行的实际上下文将虚拟地址转换为物理内存地址，才能实现对真实内存数据的操作。
这个转换一般由一个叫MMU（Memory Management Unit）的硬件完成。
而同一时间能运行的进程总数有MMU个数决定(基本上是1)。
切换上下文的重大开销之一就是 配置该进程的MMU, 重新建立起映射关系。



页管理映射:
4G物理内存:       | <--- 页id  20bit ---> | <--- 偏移 12bit ---> |
32虚拟内存:       | <--- 页id  20bit ---> | <--- 偏移 12bit ---> |
64虚拟内存: | <----- 页id  52bit -------> | <--- 偏移 12bit ---> |
内核实际上只用了 47 bit的虚拟内存; 即 35bit 的 页id.

偏移 12bit 即 4K 最小内存管理单元. 剩下的 bit 用来表示 页id
这个 4K 是生效在 MMU 的硬件页表中的. MMU 进行映射时, 只会把 页id 的部分进行映射.
"映射"的物理逻辑比较复杂(包括 缓存 / 优化等). http://en.wikipedia.org/wiki/Page_table

swap 分区 和 页缺失
物理内存不够用的时候; 会把部分物理内存页数据拷贝到 swap 分区的磁盘中.
同时删除进程上下文 在 MMU中的页表项, 进程本身的page对象要保留
当机器码继续运行, MMU在工作时, 如果页表中某个内存页找不到(此处是 虚拟地址找不到, 即没有映射)
此时会触发一个缺页异常, 调整内核注册的异常回调,
并把磁盘中相应的地方将磁盘页载入到内存中, 重新把页面写到 MMU中
然后重新执行由于缺页而失败的机器指令.
如果物理内存不够, 需要再把另一部分放到swap分区中. <<深入理解计算机系统>>
然后标记相应进程的page 需要在 下次切换时删除 MMU中的页表项



进程的虚拟内存地址空间，只有按页映射到物理内存地址，才能真正使用。
受物理存储容量限制，整个堆虚拟内存空间不可能全部映射到实际的物理内存
内核中; 每个进程维护一个break指针 (task_struct->brk ??) 指向堆空间的地址。
从堆起始地址到break之间的地址空间为映射好的,
而从break往上，是未映射的地址空间，如果访问这段空间则程序会报错.
程序要申请更大的堆空间, 则需要把break偏移往上移, 然后内核造表, 写入MMU.
注意, 每次上移的是最小单元4K
```c++
int brk(void *addr);            // 设置 break 指针
void *sbrk(intptr_t increment); // 上移 break 指针; return 当前break地址

curr_brk = sbrk(0); // 参数0, 用以获取 break 地址
```
物理上 break 指针不能上移(堆溢出), 在临近 library-mapping 地址段之间, 
还有一个"堆空间限制" (`task_struct->`) , 当然也有一定的上移空间
```c++
struct rlimit rlimit_as;
getrlimit(RLIMIT_AS, &rlimit_as);
rlimit_as->rlim_cur;
rlimit_as->rlim_max;
setrlimit(RLIMIT_AS, &rlimit_as); // 只能设置 rlim_cur
```
为了减少用户进程维护 4K,4K,4K增长的堆空间大小; 一般语言的标准库都有自己的维护方法

# 内存分配算法1: block 链表法
## 查找block ( malloc )
1. 一般来说有两种查找算法：
First fit：从头开始，使用第一个数据区大小大于要求size的块所谓此次分配的块
Best fit：从头开始，遍历所有块，使用数据区大小大于size且差值最小的块作为此次分配的块
2. 找不到, 就新建
3. 新建的时候发现空间不够, 然后就使用brk

## 新建block ( malloc )
1. 如果现有block都不能满足size的要求，则需要在链表最后开辟一个新的block。这里关键是如何只使用sbrk创建一个struct：

## 分裂block
First fit有一个比较致命的缺点，就是可能会让很小的size占据很大的一块block, (第一个找到了)
此时，为了提高payload，应该在剩余数据区足够大的情况下，将其分裂为一个新的block

## 释放block ( free ) 这玩意很复杂的
### 传入地址有效性问题
1. 地址在堆底 与 堆顶break之间
2. 地址确实是block的地址
```c++
// 实际上每个block头, 中记录block数据地址头
void free(void * addr) {
    struct alloc_block * alloc_block;
    if (addr < first_entry(block_lists)->data || brk < addr) // 非法范围
        return;
    alloc_block = addr - sizeof(alloc_block);
    if (alloc_block->data != addr)               // 方法1
        return;
    if (alloc_block->magic != ALLOC_BLOCK_MAGIC) // 方法2
        return;
}
```
3. 当然, block 可能没有保存返回给用户的地址.

### 尽量降低碎片问题
1. free某个block时，如果发现它相邻的block也是free的，则将block和相邻block合并
    + 为此; block_lists 必须是双向链表
2. 

## realloc 优化点
### 如果当前block的数据区大于等于realloc所要求的size，则不做任何操作
### 如果新的size变小了，考虑split, split出来的执行与后面内存块的合并逻辑
### 如果当前block的数据区不能满足size，但是其后继block是free的，并且合并后可以满足，则考虑做合并

## 优化: 大内存使用 mmap (而不是brk)
## 优化: 拆分多个尺寸的 block_lists ( 4K, 8K，16K, 32K, 64K, ... 1048K ) (buddy system)
此时可以根据size到对应链表中做分配，可以有效减少碎片，并提高查询block的速度
形同内核的 buddy system
尺寸拆分
1. 假设要申请一个256个页框的块
    1. 先从256个页框的链表中查找空闲块
    2. 如果没有，就去512个页框的链表中找
    3. 找到了则将页框块分为2个256个页框的块
    4. 一个分配给应用，另外一个移到256个页框的链表中
    5. 如果没有, 继续向1024个页框的链表查找
    6. 找到了则将页框块分为2个256个页框的块 和 1个512页框 (返回用户 和 移到相应链表)
    7. 如果仍然没有，则返回错误。

## 优化: 只存放free的block，而不存放已分配的block，可以减少查找block的次数，提高效率











