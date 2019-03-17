# gdb dashboard

+ gdbtui 分屏显示 asm/src/cmd/reg 但是没有语法高亮
  + focus cmd/src/reg/asm 控制窗口焦点
  + 方向键 浏览 src/asm
+ cgdb 分屏显示 src 有语法高亮  但无分屏asm
  + (无法和tmux共用) 
  + Exc 进入浏览源码vim模式
  + i   进入gdb命令模式

xxgdb  把b命令button化而已
神器 https://github.com/cyrus-and/gdb-dashboard

### 高阶watch
dashboard expressions watch expression
dashboard expressions unwatch num
dashboard expressions clear

### 源码浏览 (无法 上下左右  浏览)
dashboard source -output /dev/pts0  设置模块输出tty
dashboard source -style context 10  设置上下文多少行

### 一些常用的
dashboard source -style context 20
dashboard stack  -style limit 10
dashboard stack  -style locals True / False