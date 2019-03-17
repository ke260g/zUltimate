### Core文件

+ /proc/sys/kernel/core_pattern
  + 记录程序生成的core的路径名 默认是core
  + 如果是相对路径 怎在程序crash时的
+ file 可以看到那个文件是core
+ readelf -h 文件名 可以看到 Type: Core
+ 开启core
  + ulimit -c number // core stack block size bytes
  + ulimit -c unlimited
+ gdb 中 file corefilename 即可进入调试阶段
+ /proc/sys/kernel/core_pattern 的format
%%
%p pid
%u uid
%g gid
%e 执行文件名
%s signal
%t 时间
%h 主机名