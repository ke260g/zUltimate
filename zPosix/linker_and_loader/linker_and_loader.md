https://linker.iecc.com/
https://wh0rd.org/books/linkers-and-loaders-1.tar.lzma

1. 链接 和 加载 流程的关键阶段
2. 这些流程中涉及到的关键的对象
3. 这些对象, 在这些阶段中 又有什么关联


理解 .o, .exe / .so 文件的格式, struct 定义,
    有哪些 segment, 分别又有什么作用
理解 .o 文件如何被解析的,
    ld 读取后, 构造了什么全局对象, 这些全局对象如何交互?
    这些全局对象 中间 经历了哪些 关键的流程, 最终如何产出 .exe / .so
理解 .exe / .so 如何被解析的
    exec 读取后, 构造了什么全局对象, 这些全局对象如何交互?
    这些全局对象 中间 经历了哪些 关键的流程, 最终如何加载到 内存中的?

链接 和 加载 底层是两个"重定向"过程

1. chapter 01 编译流程的简介
    1. (ignore) linker and loader 发展历史
    2. (ignore) linker 的传参方式 4 种
2. chapter 02 一些 CPU 架构的细节讨论
3. chapter 03 object file 的理论设计
    + 即 elf 文件各 struct 的理论推导

加载流程:
1. .text 先查找一下是否已经映射了, 如果那就创建虚拟内存映射过去,
         否则创建虚拟内存后, 加载
2. .data 申请并映射, 然后内核根 elf 文件内存对这块内存初始化
3. .bss  申请并映射, 且初始化为0, 这部分在 elf 文件中只有大小说明
4. stack 初始化一个栈的大小(最小的栈 是多少?)
5. heap  堆内存是不会提前初始化; 写一个没有malloc的程序即可验证


section 是 linker 去解析的
segment 是 loader 去解析的
segment 包含若干个 section


将要研究的结构
cat /usr/include/elf.h  | grep "Elf32_[a-zA-Z]*;"
Elf32_Ehdr;
Elf32_Shdr;
Elf32_Chdr;
Elf32_Sym;
Elf32_Syminfo;
Elf32_Rel;
Elf32_Rela;
Elf32_Phdr;
Elf32_Dyn;
Elf32_Verdef;
Elf32_Verdaux;
Elf32_Verneed;
Elf32_Vernaux;
Elf32_Nhdr;
Elf32_Move;
Elf32_gptab;
Elf32_RegInfo;
Elf32_Lib;