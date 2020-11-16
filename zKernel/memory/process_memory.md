[TOC]
进程的的内存管理; 主要考虑两个入口, brk 和 exit


SYSCALL_DEFINE1(exit, int, error_code)
do_exit
exit_mm
    > mm_release  (释放 task->mm 页链表)
    > 其他子函数看不懂, 高亮一下 mm 这个变量

SYSCALL_DEFINE1(brk, unsigned long, brk)
do_brk
    > vma = kmem_cache_zalloc(vm_area_cachep, GFP_KERNEL); // 申请了内存, 放到 vma
    > vma_link(mm, vma, prev, rb_link, rb_parent); // 放到 task->mm 页链表里面





