[TOC]
# syscall

1. 每个系统分配了一个 系统调用号(以 系统调用表存储); libc 通过系统调用函数; 传递系统调用号 到内核态
2. 声明 `arch/$arch/include/asm/syscall.h`
3. `extern const sys_call_ptr_t sys_call_table[];` 系统调用表
定义:
`arch/x86/kernel/syscall_32.c`
`arch/x86/kernel/syscall_64.c` amd64
`arch/arm/kernel/calls.S`      arm
`arch/arm64/kernel/sys.c`      arm64

所有系统调用的模板(架构无关) `include/uapi/asm-generic/unistd.h`
每个系统的内核实现函数是 sys_$syscall;
实际上内核源码里面是搜不到的; 要用正则搜索 `SYSCALL_DEFINE.\($syscall`

系统调用的入口是 软中断处理程; 软中断本质上是CPU 中断的一种; 即中断回调
系统调用的 中断处理回调函数名一般为 system_call (部分架构可以不遵循)
该函数一般用 汇编写; 包括 系统调用号; 寄存器保存; 参数入栈等;

x86 中预定义中断号为 128 作为软中断
x86 中通过汇编指令 `int $0x80` 触发软中断; 跳转到128中断号的中断回调中
x86 system_call 中断回调的实现位置在 `arch/x86/kernel/entry_32.S`中
    `ENTRY(system_call)` 与 `ENDPROC(system_call)` 之间
x86 system_call 中断回调的注册位置在 
    `arch/x86/kernel/traps.c::trap_init()` 其中调用 set_system_trap_gate()`

分析 x86 的 system_call
    `SAVE_ALL`                      传参; 从右往左把 edi esi edx ecx ebx 5个寄存器依次入栈 看不大懂
    `jnz syscall_trace_entry`       系统调用跟踪程序
    `call *sys_call_table(,%eax,4)` 系统调用号在 %eax 寄存器中; 跳转到系统调用
    `movl %eax,PT_EAX(%esp)`        把返回值放在 %eax 寄存器中; 返回给用户态

一般的系统调用的参数都少于 6个;
传参的实现还依赖 编译器 的实现

添加一个系统调用的步奏
1. 在arch 的 sys_call_table 中添加一项
2. 在arch 的 `asm/unistd.h` 中新增系统调用号; 必须与 sys_call_table 位置对应
3. 用户态 源码使用 `_syscall$num(参数1类型, 参数2类型, 参数3类型, 系统调用名称)` 声明系统调用
    1. $num 为系统调用参数个数; 支持 `[0, 6)`
    2. 