https://linker.iecc.com/
https://wh0rd.org/books/linkers-and-loaders-1.tar.lzma

precompile > compile > assemble > link
预编译 > 编译 > 汇编 > 链接
其中每个流程, 都是要走两遍的, two pass

1. 链接 和 加载 流程的关键阶段
2. 这些流程中涉及到的关键的对象
3. 这些对象, 在这些阶段中 又有什么关联

# Q: 不同符 的 地址绑定 时机
1. .so: 程序加载时, 连同程序一起绑定; 甚至 在符号首次被 调用时绑定
2. .o: 链接时绑定
3. .exe:  运行时绑定
4. dlopen: 使得 .so 的符号地址, 在程序运行时绑定

# chapter 01 
+ 功能1: 地址绑定: 具象地址 与 抽象地址的绑定
Input:  具象地址: 函数printf, 变量a
Output: 抽象地址: .text 段的偏移 1000bytes, .data 段的偏移 1000bytes

+ 功能2: 重定向:
    1. 使得 共享库 在编码时, 可以以 0地址 作为起始地址, 而不是提前确定偏移
       且在运行时, 起始地址改变, 机械码访问变量时以 起始地址 的偏移寻址
    2. 使得 调用者 在编码时, 可以以 库符号 调用共享库的 函数,
       且在运行时, 可以自动地找到 共享库中符号的地址

历史: 
1. 无 汇编器: 直接使用机械码, 即编码时确定函数与变量地址.
    + 问题本质: 过早绑定地址, 导致修改的工作量大
2. 有 汇编器: 函数地址用 label 表示, 最后在整体汇编时, 计算地址
    + 延迟函数地址的绑定
3. 函数库: 子函数库的使用以及出现, 比汇编器还要早
    + 这里的函数库, 相当于现在的 .o, .a
4. 函数库: 底层需要 ld 实现的两个    功能:  重定向 和 库查找
    1. .o 编码时, 其实地址以 0地址为准
    2. 最后链接为整体程序时, 重新计算偏移, 

## linkder and loader 的三个主要业务
1. loading: disk 中的 elf 文件, 拷贝到内存中的过程, (loader)
   其中涉及, 申请虚拟内存, 实际内存, 设置内存页权限 的过程
2. relocation: (linder and loader)
   1. 链接阶段: 每个 .o, .so 都是以 零地址开始的, 编译成一个exe时需要重定向
   2. 运行阶段: .exe 是从零地址开始的, 但分页分段使得每段 段首地址 要求重新确定
3. symbol resolution: (linker)
   .o, .so 间通过"符号"进行调用, 链接阶段需要把这些符号, 解析为地址