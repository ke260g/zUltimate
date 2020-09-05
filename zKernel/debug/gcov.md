
# 内核 gcov 选项
CONFIG_GCOV_KERNEL=y                // recommanded
CONFIG_GCOV_FORMAT_AUTODETECT=y     // recommanded
CONFIG_GCOV_PROFILE_ALL=y           // for the entire kernel; not suggested

GCOV_PROFILE_main.o := y   // Makefile, 指定文件
GCOV_PROFILE := y          // Makefile, 指定目的
GCOV_PROFILE_main.o := n   // Makefile, CONFIG_GCOV_PROFILE_ALL=y 时, 屏蔽单个文件
GCOV_PROFILE := n          // Makefile, CONFIG_GCOV_PROFILE_ALL=y 时, 屏蔽单个目录

# 用户态工具
```sh
lcov  -z # 清理内核覆盖率
lcov -c -o kernel.info   # 统计内核所有代码覆盖率
lcov -c -k fs -o fs.info # 指定内核代码下 fs 文件夹代码覆盖率


genhtml  fs.info -o fs.conv # 生成 html
lcov -l  fs.info # 简单查看
```












