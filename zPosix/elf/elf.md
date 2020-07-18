[TOC]
+ https://users.cs.northwestern.edu/~pdinda/icsclass/doc/elf.pdf
+ 符号表 / 重定向 / 动态链接

# 1. format
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

## 1.3 section 类型
.bss
.text
.rodata


SHT_PROGBITS
SHT_SYMTAB
SHT_STRTAB
SHT_RELA
SHT_HASH
SHT_DYNAMIC
SHT_NOTE               
SHT_REL                : .rela.dyn / .rela.plt
SHT_SHLIB
SHT_SYNSYM             : .dynsym / .symtab
SHT_LOPROC             : .dynamic, SharedLibrary
SHT_HIPROC / SHT_LOUSER:  架构相关
HIUSER

## 1.4 segment 类型
.PHDR
.INTERP
.load
.dynamic
.note
.GNU_STACK
.GNU_RELRO

# 2. Header
## 2.1 Elf Header
## 2.2 Section Header Table
## 2.3 Program header table

# 3. symbol table

# 4. redirect (program loading and dynamic linking)



# elf header
```c++
#include <elf.h>
struct ElfN_Ehdr;
struct Elf32_Phdr; // program header
struct Elf32_Shdr; // section header

```

