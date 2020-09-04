/proc/meminfo
/proc/buddyinfo
/proc/slabinfo

Since the pages are mapped to user space, they might be swapped out
只有用户空间的内存, 才会被swapped out

memory allocated with vmalloc() is not physically contiguous.

#cat /proc/meminfo
MemFree:           62028 kB
Cached:            55616 kB
Buffers:            5240 kB
Slab:              <---- is the total number of /proc/slabinfo

free(1) buffer and cached:
Buffer is something that has yet to be "written" to disk. 
Cached is something that has been "read" from the disk and stored for later use.
Theoretically, if Cached rises up then MemFree goes down, when then system is ordinary.
System will try to used more and more Cached if is available.
When allocation of memory has been stricted, Cached will be free.
There is something going to be wrong(like out-of-big-page), when Cached goes down but MemFree keep rising up at a long time.

1. 正常情况下, Cached 会尽可能地使用内存, 如果 MemFree 不够用, Cached才会一点一点释放.
2. Cached 释放得少, 且 MemFree 上涨得慢, 因为释放出来的被使用了。
3. 如果 Cached 大幅下降, 可能是因为需要大量内存, 这个时候MemFree按照理论不会大幅上涨。
4. 如果 Cached 大幅上升, 且 MemFree 大幅上升, 即 Cached 释放后 MemFree 没有被使用, 这种场景中, 很有可能是 out-of-big-page (查看 /proc/buddyinfo 确认)
5. /proc/buddyinfo 越往右, page-size 越大;
page-size: 1、2、4、8、16、32、64、128、256、512、1024

内核内存合并选项
CONFIG_COMPACTION=y