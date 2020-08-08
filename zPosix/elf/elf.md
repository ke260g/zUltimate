[TOC]
+ https://users.cs.northwestern.edu/~pdinda/icsclass/doc/elf.pdf
+ https://www.man7.org/linux/man-pages/man5/elf.5.html
+ 重定向 / 程序加载 / 动态链接
+ (program loading and dynamic linking)
+ global_offset_table
+ procedure_linkage_table:
    + 重定向函数调用到合适的地址
    + link_editor    初始化生成 一个 procedure_linkage_table
    + dynamic_linker 在执行进程时修改
+ 器件: program_interpreter / link_editor / dynamic_linker
+ 流程: redirect / program-loading / dynamic-linking
+ 表项: global_offset_table procedure_linkage_table
+ 进程image 简单来说有三个部分: 代码段 数据段(堆) 栈

process images: so-called dynamic program representations (动态程序表示)
program loading: 给定 exe 文件, 内核把该exe加载内存中的过程
dynamic linking: exe加载到内存中后, 解析exe / so 引用的符号的过程

# 1. Overview
## 1.1 3 types files (.o / .so / .exe)
1.   .o: relocatable   与其他.o 生成 .exe 或者 .so
2. .exe: executable    用以创建运行在内存中的 进程镜像
3.  .so: shared object 用以与其他.o/.so 创建.exe; 用以与 .exe/其他.so创建 进程镜像
## 1.2 2 perspective （linking / execution)
```txt
Linking View                             Execution View
+----------------------------------+     +----------------------------------+
| program header table (optional)  |     | program header table (recommand) |
+----------------------------------+     +----------------------------------+
|  Section 1                       |     |  Segment 1                       |
|  Section ...                     |     |                                  |
+----------------------------------+     +----------------------------------+
|  Section n                       |     |  Segment 2                       |
|  Section ...                     |     |                                  |
+----------------------------------+     +----------------------------------+
|  ..                              |     | ...                              +
+----------------------------------+     +----------------------------------+
| Section Header Table (recommand) |     | Section Header Table (optional)  |
+----------------------------------+     +----------------------------------+
```
+ section 是独立的;  一个 segment 包含多个section
+ section header: 描述 section 的表; 在文件结尾;
+ program header: 描述 segment 的表; 在文件开始; 紧接着 elf header
+ 所以有个 section <-> segment 的映射关系; `readelf | grep "Section to Segment mapping"`

## 1.3 section
| name      | sh_type      | sh_flags | functionality |
| :--       | :--          | :--      | :--           |
| .bss      | SHT_NOBITS   | SHF_ALLOC SHF_WRITE | 全局非初始化变量; 不占用elf空间 |
| .data     | SHT_PROGBITS | SHF_ALLOC SHF_WRITE | 进程的初始化数据 |
| .rodata   | SHT_PROGBITS | SHF_ALLOC | 进程的只读数据 |
| .text     | SHT_PROGBITS | SHF_ALLOC SHF_EXECINSTR | 大部分机械码 |
| .init     | SHT_PROGBITS | SHF_ALLOC SHF_EXECINSTR | `__attribute__((constructor))` |
| .fini     | SHT_PROGBITS | SHF_ALLOC SHF_EXECINSTR | `__attribute__((destructor))`; atexit() |
| .got      | SHT_PROGBITS || global offset table |
| .plt      | SHT_PROGBITS || procedure linkage table |
| .interp   | SHT_PROGBITS | | 解析器的路径 |
| .line     | SHT_PROGBITS | | 用于gdb; 机械码对应的行号 |
| .debug    | SHT_PROGBITS | | 用于gdb; 调试符号信息 |
| .comment  | SHT_PROGBITS | | 版本信息 |
| .dynamic  | SHT_DYNAMIC  ||动态链接-信息|
| .dynstr   | SHT_STRTAB   | SHF_ALLOC | 动态链接-字符串信息(指示符号表) |
| .dynsym   | SHT_DYNSYM   | SHF_ALLOC | 动态链接-符号表 |
| .hash     | SHT_HASH     | SHF_ALLOC | symbol 哈希表 |
| .note     | SHT_NOTE     | | ?? |
| .rel.X    | SHT_REL      |  | 用以重定向 |
| .rela.X   | SHT_RELA     | | 用以重定向 |
| .shstrtab | SHT_STRTAB   | | section's name |
| .strtab   | SHT_STRTAB   || 与symtab关联的字符串 |
| .symtab   | SHT_SYMTAB   || 符号表 |

.got.plt
.rela.dyn / .rela.plt /  .rela.rodata

## 1.4 segment 类型
.PHDR
.INTERP
.load
.dynamic
.note
.GNU_STACK
.GNU_RELRO

# 2 Elf Header
# 3 Section Header (section 的 类型定义)
```c++
/**
 * @sh_name:  name (如 .text/.data/.bss)
 * @sh_flags: 权限 (写/申请/执行; 默认自带 读)
 * @sh_type:
 */
typedef struct {
    uint32_t   sh_name;
    uint32_t   sh_type;
    uint32_t   sh_flags;
    Elf32_Addr sh_addr;
    Elf32_Off  sh_offset;
    uint32_t   sh_size;
    uint32_t   sh_link;
    uint32_t   sh_info;
    uint32_t   sh_addralign;
    uint32_t   sh_entsize;
} Elf32_Shdr;
```
## 3.1 sh_flags  (实际上是执行权限)
    + SHF_WRITE:      section should be writable during process execution
    + SHF_ALLOC:      section will allocate memory during process execution
    + SHF_EXECINSTR:  section should be executable (holds machine code)
## 3.2 sh_type , sh_link, sh_info 的关联性
+ loader 根据sh_type解析sh_link/sh_info

| sh_type | sh_link | sh_info |
| --: | :-- | :-- |
| SHT_DYNAMIC | index of SHT_STRTAB used by this section | |
| SHT_HASH    | index of symbol-table applied to by this section | |
| SHT_REL SHT_RELA | index of the associated symbol-table | index of the section applied to by this section |
| SHT_SYMTAB SHT_DYNSYM | index of the associated SHT_STRTAB | 1 + (index of the last STB_LOCAL) |

## 3.3 sh_type (section header type)
+ SHT_NULL                : indicates a inactive entry
+ SHT_PROGBITS            : executable code
+ SHT_SYMTAB SHT_DYNSYM   : symbol table                          (only one)
+ SHT_STRTAB              : string table                          (multiple)
+ SHT_RELA                : relocation with explicit addends      (multiple)
+ SHT_HASH                : symbol hash table for dynamic linking (only one)
+ SHT_DYNAMIC             : dynamic linking infomation            (only one)
+ SHT_NOTE                : 
+ SHT_NOBITS              : occupies no space in elf; but need for execuation
    + 正是因为这类型的 .section 导致 Elf32_Phdr->p_memsz > Elf32_Phdr->p_filesz
+ SHT_REL                 : relocation without explicit addends   (multiple)
+ SHT_SHLIB               : reverse
+ SHT_LOPROC SHT_HIPROC   :
+ SHT_LOUSER
+ SHT_HIUSER



# 3. String Table (.synstr)
1. 就是以 null 终止的字符串序列; 形同 `char *s[]`;
2. 用以指示elf文件中的字符串, 索引方式为 .synstr section 的偏移值
3. 引用实例:
    + setion头 Elf32_Shdr->name
    + dynsym头 Elf32_Sym->name
+ 与程序运行过程中的 字符串无关; 仅用于解析 elf
+ 没有被引用的字符串; 不允许没被引用的字符串


# 4. symbol table (.dynsym .symtab)
1. 符号表的本质是一个数组 `struct Elf32_Sym sym[];`
2. 符号表的首个符号必须是 `{ .st_info = 0, .st_size = 0, .st_value = 0 }`
    + 即 ELF32_ST_INFO(STB_LOCAL, STT_NOTYPE
## 4.1 定义 struct Elf32_Sym
```c++
/**
 * @st_name:  name (外引用变量/函数/)
 */
typedef struct {
    uint32_t      st_name;
    Elf32_Addr    st_value;
    uint32_t      st_size;
    unsigned char st_info;
    unsigned char st_other; // reverse
    uint16_t      st_shndx; // 该符号关联的 section header index
} Elf32_Sym;
#define ELF32_ST_BIND(info)       // get bind (属性)
#define ELF32_ST_TYPE(info)       // get type (类型)
```
## 4.2 Elf32_Sym->st_info (属性 和 类型)
### 4.2.1 Elf32_Sym->st_info->ELF32_ST_BIND 属性
```c++
#define STB_LOCAL  0 // 仅用于本elf文件(static 变量和函数)
#define STB_GLOBAL 1 // 所有elf文件共享; st_info->name 链接时全局唯一
#define STB_WEAK   2 // 低优先级的 STB_GLOBAL; 链接时可以不唯一
```
+ 如果 global 变量类型 和 local变量冲突; 在进程镜像中, 内核如何处理???
    + 能否编译通过??
+ STB_WEAK 和 STB_GLOBAL 的区别
    1. STB_WEAK 与 SHN_COMMON 同名不会产生冲突 
    2. 与 STB_GLOBAL 同名不会产生冲突
    3. 链接器抽取 .a 时; 将抽取 在 .o 中的STB_GLOBAL符号; 可以是 .a 中的STB_WEAK 或 STB_GLOABL
    4. 链接器抽取 .a 时; 在 .o 中的STB_WEAK不会得到解析; 且 .o 中的 st_value 置为0
    5. STB_LOCAL 排列在 STB_GLOBAL STB_WEAK的前面; Elf32_Shdr->sh_info 指示 last STB_LOCAL 的
    6. 因此, 符号表构建时; 优先构建 STB_LOCAL; 再构建 STB_GLOBAL / STB_WEAK
+ STB_WEAK 的 使用场景??
    1. ??
### 4.2.2 Elf32_Sym->st_info->ELF32_ST_TYPE 类型
```c++
#define STT_NOTYPE  0
#define STT_OBJECT  1 // 变量
#define STT_FUNC    2 // 函数
#define STT_SECTION 3 // section; 用于重定向; bind = STB_LOCAL
#define STT_FILE    4 // st_name = 源文件路径; 先于其他 STB_LOCAL; bind STB_LOCAL; st_shndx = SHN_ABS
#define STT_COMMON  5 // 
#define STT_TLS     6

```
1. STT_FUNC 在 .so 有特殊含义: .so 中的 STT_FUNC 被其他 .o 文件引用时,
   ld 创建 "procedure linkage table", .so 的其他符号不会被 "procedure linkage table" 引用

### 4.3 st_shndx
1. 该符号引用 引用其他 section
2. 重定向过程中, 随着section的移动, st_shndx 随着变化, st_value 也是
```c++
#define SHN_UNDEF  // 该符号 未定义; 指示 extern 变量, 外部函数
#define SHN_ABS    // 该符号 st_value 重定向不变
#define SHN_COMMON // 该符号 需要alloc; st_value 地址对齐值(比如4); st_size alloc 的大小;
                   // 一般是 数组, 如 int a[100]; 
```
3. 非标准定义的 st_shndx 指示 elf 文件中的其他section

### 4.4 st_value
1. 重定向前(.o) SHN_COMMON 的符号, st_vaslue 表示分配地址的对齐值
2. 重定向前(.o) st_value 指示 "st_shndx指示的section" 的entry索引
3. 重定向后(.so .exe) 指示虚拟地址; `exec*()` 加载器需要处理 虚地址冲突问题

# 5. redirect (.rel .rela)
+ 重定向是指: .o + .so = .exe;  .o + .so = .so
+ 重定向产出: 被修改的 正确的 section (尤其是 其中的 address)
+ 重定向具体步骤:
    1. 编译和定为 输入文件 (即 具体哪个.o)
    2. 更新符号表 st_value
    3. 重定向
+ 重定向section 引用另外两个 section: 
    1. .symtab(由sh_info指示)
    2. 将要修改的 section (由 sh_link 指示)
## 5.1  定义 Elf32_Rel Elf32_Rela
```c++
typedef struct elf32_rel {
  Elf32_Addr    r_offset; 
  Elf32_Word    r_info;
} Elf32_Rel;

/**
 * @r_offset: 重定向前: 符号所在section的bytes偏移; 重定向后: 符号的虚拟地址
 * @r_info:   关联符号在符号表的索引 & 重定向的类型(指示 如果使用 r_offset 进行计算)
 * @r_addend: 用于计算重定向后的 st_value 的常量附加值
 */
typedef struct elf32_rela {
  Elf32_Addr    r_offset;  /* Location at which to apply the action */
  Elf32_Word    r_info;    /* index and type of relocation */
  Elf32_Sword   r_addend;  /* Constant addend used to compute value */
} Elf32_Rela;

#define ELF32_R_SYM(x) ((x) >> 8)    // get 符号的索引 (.symtab[index])
#define ELF32_R_TYPE(x) ((x) & 0xff) // get 重定向类型
```

## 5.2 Elf32_Rela->r_info (架构相关; 以x86为例)
最终计算出 **relocation field**
### 5.2.1 relocation notation:
A: r_addend
B: base address; .so 加载到内存中的基地址(/proc/$pid/maps); 一般是0
G: 符号所在的 global_offset_table 的偏移;
GOT: global_offset_table 的 地址
L: 关联符号所在 procedure_linkage_table entry 的 地址or偏移
P: r_offset
S: st_value
### 5.2.2 relocation types definition
```c++
#define R_386_NONE         0            /* No reloc */                           // none
#define R_386_32           1            /* Direct 32 bit  */                     // A + S    
#define R_386_PC32         2            /* PC relative 32 bit */                 // A + S - P
#define R_386_GOT32        3            /* 32 bit GOT entry */                   // A + G - P     构造 GOT
#define R_386_PLT32        4            /* 32 bit PLT address */                 // A + L - P     构造 PLT
#define R_386_COPY         5            /* Copy symbol at runtime */             // none
#define R_386_GLOB_DAT     6            /* Create GOT entry */                   // A + S
#define R_386_JMP_SLOT     7            /* Create PLT entry */                   // A + S
#define R_386_RELATIVE     8            /* Adjust by program base */             // A + B
#define R_386_GOTOFF       9            /* 32 bit offset to GOT */               // A + S - GOT
#define R_386_GOTPC        10           /* 32 bit PC relative offset to GOT */   // A + GOT - P
```
### 5.2.3 relocation explanation
R_386_GOT32:    用来构造 GOT
R_386_GOTOFF:   用以构造 GOT
R_386_GOTPC:    用来构造 GOT
R_386_PLT32:    用来构造 PLT
R_386_COPY:     (用于动态链接); 指向可写segment; 关联符号同时存在与 .so 和 .o 中; 
                运行时 加载器把.so 中的符号表中的符号数据 copy 到 r_offset
R_386_GLOB_DAT: 指向 GOT 的一个表项
R_386_JMP_SLOT: (用于动态链接); 指向 PLT 的一个表项
R_386_RELATIVE: (用于动态链接); ELF32_R_SYM 必须是0; r_offset 指向 .so 的相对地址


# elf header
```c++
#include <elf.h>
struct ElfN_Ehdr;
struct Elf32_Phdr; // program header
struct Elf32_Shdr; // section header

```


# Program header
## 定义
```c++
typedef struct elf32_phdr{
  Elf32_Word    p_type;
  Elf32_Off     p_offset; // segment 位置; bytes offset from the beginning of the file;
  Elf32_Addr    p_vaddr;  // segment 运行时虚地址
  Elf32_Addr    p_paddr;  // segment 运行时实地址 (该成员是保留的; 操作系统一般都无视)
  Elf32_Word    p_filesz; // segment 在elf文件中的长度
  Elf32_Word    p_memsz;  // segment 长度; 运行在内存中时需要的大小
  Elf32_Word    p_flags;  // segment 运行时; page 的权限
  Elf32_Word    p_align;  // 对齐值
} Elf32_Phdr;

```
## Elf32_Phdr->p_flags 权限
```c++
#define PF_R            0x4
#define PF_W            0x2
#define PF_X            0x1
```

## Elf32_Phdr->p_type
```c++
#define PT_NULL         0               /* Program header table entry unused */
#define PT_LOAD         1               /* Loadable program segment */
#define PT_DYNAMIC      2               /* Dynamic linking information */
#define PT_INTERP       3               /* Program interpreter */
#define PT_NOTE         4               /* Auxiliary information */
#define PT_SHLIB        5               /* Reserved */
#define PT_PHDR         6               /* Entry for header table itself */
#define PT_TLS          7               /* Thread-local storage segment */
#define PT_NUM          8               /* Number of defined types */
#define PT_LOOS         0x60000000      /* Start of OS-specific */
#define PT_GNU_EH_FRAME 0x6474e550      /* GCC .eh_frame_hdr segment */
#define PT_GNU_STACK    0x6474e551      /* Indicates stack executability */
#define PT_GNU_RELRO    0x6474e552      /* Read-only after relocation */
#define PT_LOSUNW       0x6ffffffa
#define PT_SUNWBSS      0x6ffffffa      /* Sun Specific segment */
#define PT_SUNWSTACK    0x6ffffffb      /* Stack segment */
#define PT_HISUNW       0x6fffffff
#define PT_HIOS         0x6fffffff      /* End of OS-specific */
#define PT_LOPROC       0x70000000      /* Start of processor-specific */
#define PT_HIPROC       0x7fffffff      /* End of processor-specific */
```
1. PT_LOAD:
    1. 把elf中 Elf32_Phdr->p_filesz 大小映射到内存中
    2. 如果 `p_memsz - p_filesz` 多出的部分, 在内存中申请并初始化为0 (p_filesz <= p_memsz)
    3. PT_LOAD 的 program header 在elf中以 p_vaddr 的升序排列
    4. 导致 `p_memsz > p_filesz` 的有 .bss 这个section
2. PT_DYNAMIC:  指示 dynamic linking 的信息
3. PT_INTERP:   指向 elf 中到字符串 (.strtab); 指示解析器的绝对路径
4. PT_NOTE:     指示 aunxiliary information 的大小和信息
5. PT_SHLIB:    保留字段(估计是 历史开发阶段用以区分 .so 和 .exe的; 后来发现不需要)
6. PT_PHDR:     指向 Program Header Table 本身

## Elf32_Phdr->p_flags

# 99. PROGRAM LOADING AND DYNAMIC LINKING


# conception
## 基地址  Base Address
1. 每一个 .so, .exe 在进程执行时都有一个基地址
2. 即 .so / .exe 在内存中的起始地址; 用于在动态链接过程中, 镜像在内存中重定向
3. 基地址是在 `exec*()` 调用时计算, 决定参数有3个
    1. 内存加载地址
    2. 最大的页的大小
    3. loadable segment 的最低位虚拟地址

## 程序加载 Program Loading (以下的说明是以同一个 elf作为例子; 不代表实际代码)
1. 创建进程时, elf 文件 logically copied to memory (有 page; 不一定有实际内存映射)
2. 只有当进程在执行过程中, 引用 logical-page 才会需要 physical-page
3. 实际上, 进程在运行过程中, 确实存在 还没有被引用的 页
4. 因此, 延迟 physical-page reading, 即延迟 mem-pages 的映射, 有效提高系统性能
5. 为实现 此目的; segment 在elf中的运行时虚地址 通常是 页大小的倍数
### 程序加载 segment perspective
```txt
File Offset                 File              Virtual Address
                     +--------------------+
0                    | ELF Header         |
                     +--------------------+
Program header table | Other Information  |
                     +--------------------+
0x100                | .text segment      |   0x8048100
                     | ...                |
                     | 0x2be00 bytes      |   0x8073eff
                     +--------------------+
0x2bf00              | .data segment      |   0x8074f00
                     | ...                |
                     | 0x4e00 byes        |   0x8079cff
                     +--------------------+
0x30d00              | Other information  |
                     +--------------------+

如图:
.text 的地址 0x8048100 / ( 4 * 1024 * 1024 ) = 32; 4K
.data 的地址 0x8074f00 / ( 4 * 1024 * 1024 ) = 32; 4K
显然 .data 的地址, 不是紧接着 .text 的结尾
```
### 进程镜像 segment 分布
```txt
Virtual Address    Contents          Segment
                 +----------------+  Text-pages
0x8048000        | Header padding |  
                 | 0x100 bytes    |
                 +----------------+
0x8048100        | Text Segment   |
                 | ...            |
0x8073eff        | 0x2be00 bytes  |
                 +----------------+
0x8073f00        | padding        |
                 | begin of Data  |
                 +----------------+

0x8074000        +----------------+  Data-pages
                 | Text padding   |
                 | 0xf00 bytes    |
                 +----------------+
0x8074f00        | Data Segment   |
                 | ...            |
0x8079cff        | 0x4e00         |
                 +----------------+
0x8079d00        | Uninitialized  |
                 | 0x1024 0 bytes |
                 +----------------+
0x807ad24        | Page padding   |
                 | 0x2dc 0 bytes  |
                 +----------------+
```

1. 首个 Text page 包含 Elf32_Ehdr; program header table; 和其他信息
2. 末尾 Text page 包含 Data segment 的开始一部分;
    + 我猜是实现时, memcpy(p_offset, round_4k(p_memsz)), 才导致 .data 的开始被复制上来
3. 首个 Data page 包含 Text segment 的末尾一部分
4. 末尾 Data page 可能包含与运行进程无关的信息

### 位置无关码 位置相关码
1. .exe 有位置相关码, Elf32_Phdr->p_vaddr 在程序加载时, 不变 (真的吗? 待考证?)
2. .so 则必须是位置无关码