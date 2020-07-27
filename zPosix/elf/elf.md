[TOC]
+ https://users.cs.northwestern.edu/~pdinda/icsclass/doc/elf.pdf
+ https://www.man7.org/linux/man-pages/man5/elf.5.html
+ 符号表 / 重定向 / 动态链接
+ (program loading and dynamic linking)
+ global_offset_table
+ procedure_linkage_table:
    + 重定向函数调用到合适的地址
    + link_editor    初始化生成 一个 procedure_linkage_table
    + dynamic_linker 在执行进程时修改

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
SHT_NULL                : indicates a inactive entry
SHT_PROGBITS            : executable code
SHT_SYMTAB SHT_DYNSYM   : symbol table                          (only one)
SHT_STRTAB              : string table                          (multiple)
SHT_RELA                : relocation with explicit addends      (multiple)
SHT_HASH                : symbol hash table for dynamic linking (only one)
SHT_DYNAMIC             : dynamic linking infomation            (only one)
SHT_NOTE                : 
SHT_NOBITS              : occupies no space in elf; but need for execuation
SHT_REL                 : relocation without explicit addends   (multiple)
SHT_SHLIB               : reverse
SHT_LOPROC SHT_HIPROC   :
SHT_LOUSER
SHT_HIUSER

## 2.3 Program header

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
#define ELF32_ST_INFO(bind, type) // set info 传入属性、类型
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
+ 重定向产出: 被修改的 正确的 section
+ 重定向具体步骤:
    1. 编译和定为 输入文件 (即 具体哪个.o)
    2. 更新符号表 st_value
    3. 重定向
## 5.1  定义 Elf32_Rel Elf32_Rela
```c++
typedef struct elf32_rel {
  Elf32_Addr    r_offset; 
  Elf32_Word    r_info;
} Elf32_Rel;

/**
 * @r_offset: 重定向前: 符号所在section的bytes偏移; 重定向后: 符号的虚拟地址
 * @r_info:   关联符号在符号表的索引 & 重定向的类型
 * @r_addend: 用于计算重定向后的 st_value 的常量附加值
 */
typedef struct elf32_rela {
  Elf32_Addr    r_offset;  /* Location at which to apply the action */
  Elf32_Word    r_info;    /* index and type of relocation */
  Elf32_Sword   r_addend;  /* Constant addend used to compute value */
} Elf32_Rela;

#define ELF32_R_SYM(x) ((x) >> 8)
#define ELF32_R_TYPE(x) ((x) & 0xff)
```
## 5.2 Relocatable Fields Notations (这一节根本无法理解)
A: r_addend 参与计算 relocatable fields
B: .so 加载到内存中的 base address (/proc/$pid/maps); .so的base address一般是0
G: `global_offset_table[??offset??]->address` 将会在执行时  保持不变
GOT: global_offset_table 的 地址
L: 关联符号的 procedure_linkage_table entry
P: section-offset 或者 address 已经通过 r_offset 完成了重定向
S: value of the symbol whose index resides in the relocation entry
## 5.3 Relocation Types
+ 

# elf header
```c++
#include <elf.h>
struct ElfN_Ehdr;
struct Elf32_Phdr; // program header
struct Elf32_Shdr; // section header

```


# 99. PROGRAM LOADING AND DYNAMIC LINKING