// 稍微看下初始化流程

// arch/mips/kernel/head.S
//   init { BBS, IDT, GDT, PagesTable, Register }
//   kernel_entry() -> start_kernel()

// init/main.c:start_kernel() 
//   init { irq, memory-management, process-schedule, signal, fs, network-stack ... }
// 完整的流程比较多, 这里仅仅看看一些关键的

start_kernel()
{
    page_address_init();              // 页地址
    setup_arch(&command_line);
    page_alloc_init();                // 页分配
    jump_label_init();
    vfs_caches_init_early();          // vfs
    trap_init();                      // 系统调用
    mm_init();                        // 内存管理
    sched_init();                     // 进程调度
    init_IRQ();                       // 中断
    signals_init();
    rest_init();                      // 其他的初始化
}

// rest_init() 中
// rest_init() -> kernel_init() -> kernel_init_freeable() -> do_basic_setup()
// do_basic_setup 才是真实意义上执行 协议栈的初始化函数 
