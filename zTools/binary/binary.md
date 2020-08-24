[TOC]
# 1. binary 工具列表
| 名称      | 用途 |
| --:       | :-- |
| addr2line | 输入地址; 输出.debug 中对应的 `__FILE__` 和 `__LINE__` |
| ar        | create, modify, and extract `lib*.a` |
| as        | assembler 汇编器 |
| cpp       | c-preprocessor, 预处理器 |
| dwp       | dwarf 打包工具; 生成 mac-os 的 dwarf 包 |
| gcc       | c  编译器 |
| g++       | c++编译器 |
| gcov      | gcov 覆盖率工具 |
| gdb       | 调试工具 |
| gfortran  | fortran 编译器 |
| gprof     | profile 性能分析工具 |                               
| ld        | 链接器 |
| nm        | display object 文件中的符号; 不能用于 exe 和 so |
| objcopy   | copy 和 translate object 文件 |
| objdump   | display information from object 文件 |
| ranlib    | generate index to `lib*.a` |
| readelf   | 便捷的查询 elf 文件的工具; (实际上是 其他好工具的封装) |
| size      | display .section(不是 segment) 的大小 |
| strings   | print the strings of printable characters in files. |
| strip     | Discard symbols from object files. |

ar 创建 `lib*.a` 会生成符号的 index;
ar 可以不生成 index; 该情况下使用 ranlib 生成
ar 可以创建 **thin** 型的 `lib*.a`; 即指向相应的 .o 文件而不是拷贝后压缩 (减少 拷贝 .o 造成的时间和空间消耗)