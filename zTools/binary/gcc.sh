## get many infos, like "header search path"
gcc -v

## get library/header search path
gcc -print-multi-lib -print-search-dirs 1.c

## get linked library search path
gcc -v -Wl,--verbose

## 定义elf运行时额外的搜索路径 .rpath
-Wl,-rpath,/tmp/debug_lib
-Wl,-rpath,/extdir/debug_lib
-Wl,-rpath,/wns/3party/openssl/lib -L/wns/3party/openssl/lib
-Wl,-rpath,/wns/3party/php/lib -L/wns/3party/php/lib
-Wl,-rpath,/wns/3party/apache/lib -L/wns/3party/apache/lib
-Wl,-rpath,/wns/3party/openldap/lib -L/wns/3party/openldap/lib
-Wl,-rpath,/wns/3party/pcre/lib -L/wns/3party/pcre/lib
-Wl,-rpath,/extdir/debug_lib -Wl,-rpath,/wns/3party/lib -Wl,-rpath,/wns/lib

## 已知maps 已知crash的函数指针
1 maps找到crash的函数指针所在的so, 或者elf文件
2 计算offset = (crash函数指针 - maps中所在so的基地址)
2 反编译带符号的elf文件 objdump -DS <elf> > elf.S
3 elf.S 中找到 offset
4 addr2line -a offset -e <efl>

# so 构造


# 导出所有符号表

# https://blog.csdn.net/mw_nice/article/details/108253409?utm_medium=distribute.pc_relevant_bbs_down.none-task--2~all~first_rank_v2~rank_v25-1.nonecase&depth_1-utm_source=distribute.pc_relevant_bbs_down.none-task--2~all~first_rank_v2~rank_v25-1.nonecase
