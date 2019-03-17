# GDB
### 实际经验中 如果要调试 需要那些功能?
+ add/del 断点 ( break点 watch点 event)
+ 运行程序 执行到断点 stepinto stepover
+ 打印变量 单个变量/复杂变量
+ 变量定义
+ 堆栈
+ 跨线程调试 即停止部分线程

### 常用参数
+ gdb -q attach pid

### 常用cmd
+ 运行 run (指定参数) start(跑到main暂停) continue(跑到下个断点)
+ file 加载文件 可以是core文件 可以可执行文件
+ break 设置断点
  + break 数字  // 表示行数
  + break 字符串:数字 // 表示文件行数
  + break 字符串:字符串 // 表示文件中指定函数
  + break 字符串  // 表示函数
  + break *0x00ff // 表示函数代码地址 一般传函数地址
  + break 参数 thread tid // 可用来指定线程
+ watch 设置watch点 语法几乎同于 break
+ list 行号/函数名
  + setlistsize
  + showlistsize
  + list 参数1,参数2 // 显示范围内函数
  + +offset -offset
+ clear 删除断点
+ s step-into
+ n step-over
+ printf 字符串 字符串即c源码 继承local/global符号的
+ printf *(struct xxx *)0xfff 以可视化的形式打印结构体
+ breaktrace 堆栈
  + -fstack-protector
  + -fstack-protector-all 保护coredump堆栈 有时堆栈被破坏 看不到
+ info func 函数名 // 用来找函数实现位置 以及类型 默认进行regex
  + 然后根据位置 文件:行数 add断点
+ info breakpoints // 已经设置的断点
+ info frame 数字 // 指定frames
  + breaktrace中每个函数的栈空间即一个frames
+ info all-registers // 寄存器
+ info regs
+ info reg xxx // 查看制定寄存器
  + `info reg $pic/eab/$rsp/$rip` 例子

### examine / printf 's format
t 二进制
o 八进制
x 十六进制 a 十六进制有符号
d 十进制   u 十进制无符号
i 指令地址 (常用)
c 字符
f 浮点数

### examine x 详解直接显示内存block
+ examine 缩写为 x
+ 模式 `x /<number><format><unit>` 
+ number 表示数字 表示要看内存后多少个unit
+ unit:
b 表示byte  (1 byte)
h 表示short (2 byte)
w 表示int   (4 byte)
g 表示long  (8 byte)
+ format 根据单元显示format

### printf 高级用途 (动态数组 动态结构体)
+ printf 字符串 字符串即c源码 继承local/global符号的
+ printf *(struct xxx *)0xfff 以可视化的形式打印结构体
+ printf *0xffff@10 
+ printf funcname::staic_variable 函数局部变量名

### 跨线程调试
+ 需求: 暂停部分线程 & 其他线程继续运行
+ 初始化设置 + 在指定线程设置断点 + 跳转指定线程 + 继续执行指定线程
+ 场景模拟: 暂停部分线程{a b c} 在线程a中运行一段代码 跳转到线程b运行一段代码
```sh
# 初始化
set target-async 1
set pagination off
set non-stop on

break file:line thread $threadId # 设置断点
thread $threadId # 跳转线程 必须是调试状态的
thread apply $threadId1 $threadId2 continue # 批量 继续执行制定线程
```

### 跨进程控制: follow-fork-mode/detach-on-fork
+ set detach-on-fork on/off
    + on:  断开调试follow-fork-mode指定的进程
    + off: gdb将控制父进程和子进程(没有调试的则suspend, 这样导致crash)
+ set follow-fork-mode parent/child
    + parent: fork之后继续调试父进程，子进程不受影响
    + child: fork之后调试子进程，父进程不受影响

### kgdb
+ 即用gdb调试内核, 利用gdbserver
+ target板子上内核开选项
+ client机子 gdb连接过去
+ git://git.kernel.org/pub/scm/utils/kernel/kgdb/agent-proxy.git
+ http://sourceware.org/gdb/current/onlinedocs/gdb/
+ http://www.sourceware.org/gdb/onlinedocs/gdbint.html
+ https://kgdb.wiki.kernel.org/
+ http://www.kernel.org/doc/htmldocs/kgdb.html
+ http://www.ibm.com/developerworks/cn/linux/l-cn-gdbmp/
+ http://www.kgdb.info/