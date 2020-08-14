[TOC]
## 
https://www.memorymanagement.org/mmref/begin.html

## 3 layer
Hardware        : RAM and memory caches
Operating System: virtual memory system
Application     :

## Application
### reasons
1. limitation of resources
2. unpredictable amount of memory usage
### task
1. Allocation
2. Recycling
### Constraints of Manager
1. CPU overhead
  维护 MM管理器的 额外CPU开销
2. Pause times
  申请 和 回收所需要的响应时间
3. Memory overhead
  内存损耗率
  + administration
  + rounding (known as internal fragmentation)
  + poor layout (known as external fragmentation)

### common problems
#### Premature frees and dangling pointers
野指针 free了后 还继续用且不导致直接crash
#### Memory leak
#### External fragmentation
the free memory can become split into many small blocks, separated by blocks still in use
but it can no longer give out big enough blocks despite having enough spare memory
#### Poor locality of reference
硬件层 对于连续内存段 能有效提高IO性能
但是如果分配的内存 相对分散 则有损性能
#### Inflexible design
#### Interface complexity

### Manual memory management
#### advantages
+ it can be easier for the programmer to understand exactly what is going on;
+ some manual memory managers perform better when there is a shortage of memory.
#### disadvantages
+ the programmer must write a lot of code to do repetitive bookkeeping of memory;
+ memory management must form a significant part of any module interface;
+ manual memory management typically requires more memory overhead per object;
+ memory management bugs are common.

### Automatic memory management
#### advantages
+ the programmer is freed to work on the actual problem;
+ module interfaces are cleaner;
+ there are fewer memory management bugs;
+ memory management is often more efficient.
#### disadvantages
+ memory may be retained because it is reachable, but won’t be used again;
+ automatic memory managers (currently) have limited availability.

### Allocation Techniques
Below techniques can often be used in combination.
+ perfect fit; best fit; good fit; worst fit;
+ sequential fit; first fit; next fit; segregated fit; indexed fit;
#### First fit
+ the allocator keeps a list of free blocks (known as the free list)
+ on receiving a request for memory, scans along the list for the first block that is large enough to satisfy the request
+ If the chosen block is significantly larger than that requested, then it is usually split, and the remainder added to the list as another free block
+  When recycling free blocks, there is **a choice** as to where to add the blocks to the free list
#### Next fit
#### Buddy system
#### Suballocators

### Glossary
仅记录一些关键的影响理解的词汇
#### coalescence
#### locality of reference