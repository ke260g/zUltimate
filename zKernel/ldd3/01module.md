1 入口 出口
#include <linux/init.h>
module_init(init_function);
module_exit(cleanup_function);
__init __initdata __exit __exitdata
2 编译

3 用户态操作
insmod rmmod lsmod modprobe
ls  /sys/module
cat /proc/modules

4 传参
```c++
#include<linux/sched.h>
struct stask_strcut *current; // object pointed to the user process 
current->pid;
current->comm;

#include <linux/moduleparam.h>
module_param(variable, type, perm);
module_param_array(name,type,num,perm); 

static char *whom = "world";
static int howmany = 1;
module_param(howmany, int, S_IRUGO);
module_param(whom, charp, S_IRUGO);
// insmod your_module whom="heaven" howmany=100
// 
```
5 模块间依赖