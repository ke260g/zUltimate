[TOC]
# 1 kernel building option for debug
CONFIG_DEBUG_KERNEL
CONFIG_DEBUG_SLAB
CONFIG_DEBUG_PAGEALLOC
CONFIG_DEBUG_SPINLOCK         // cache spin-error-log
CONFIG_DEBUG_SPINLOCK_SLEEP
CONFIG_INIT_DEBUG             // check __init/__initdata whether be writen in function
CONFIG_DEBUG_INFO             // must for gdb kernel
CONFIG_FRAME_POINTER          // suggested for kernel
CONFIG_MAGIC_SYSRQ
CONFIG_DEBUG_STACKOVERFLOW    // track down kernel stack overflow
CONFIG_DEBUG_STACK_USAGE      // track down kernel stack overflow
CONFIG_KALLSYMS               // kernel debug symbol; like gcc -g
CONFIG_IKCONFIG
CONFIG_IKCONFIG_PROC          // show full configurtion in /proc/config (useless at most time)
CONFIG_ACPI_DEBUG             // verbose acpi error print
CONFIG_DEBUG_DRIVER           // verbose driver error print （associcated with Device Model)
CONFIG_SCSI_CONSTANTS         // verbose scsi error print
CONFIG_INPUT_EVBUG            // verbose input event print
CONFIG_PROFILING              // track down kernel hangs and related problems
# 2  printk
`KERN_EMERG`    Used for emergency messages, usually those that precede a crash.
`KERN_ALERT`    A situation requiring immediate action.
`KERN_CRIT`     Critical conditions, often related to serious hardware or software failures.
`KERN_ERR`      Used to report error conditions; device drivers often useKERN_ERRto report hard-ware difficulties.
`KERN_WARNING`  Warnings about problematic situations that do not, in themselves, create seri-ous problems with the system.
`KERN_NOTICE`   Situations that are normal, but still worthy of note. A number of security-relatedconditions are reported at this level.
`KERN_INFO`     Informational messages. Many drivers print information about the hardwarethey find at startup time at this level.
`KERN_DEBUG`    Used for debugging messages.
+ /var/log/messages
+ loglevel control /proc/sys/kernel/printk*
+ dmesg
+ default loglevel is DEFAULT_MESSAGE_LOGLEVEL
+ /etc/syslog.conf 系统进程 一般读取这个文件; 重定向kernel-log; ( rsyslog klogd )
+ rate limit; `if (printk_ratelimit()) printk("messages");` 避免打爆内核日至缓存
## redirect printk message
```c++
char data[2];
data[0] = TIOCL_SETKMSGREDIRECT; // linux/tiocl.h
data[1] = 1;  // means tty#1, implementation is in tioclinux(); 
ioctl(STDIN_FILENO, TIOCLINUX, data);
```

# 3 /proc （ proc_fs.h + seq_file.h )
## 3.1 /proc include/linux/proc_fs.h
+ can be used for communication between user
+ can be used for debug
1. /proc dir  opertation `proc_symlink(); proc_mkdir(); remove_proc_subtree();`
2. /proc file opertation `proc_create(); proc_remove();`
3. /proc file_operations `struct proc_dir_entry->file_operations`

## 3.2 include/linux/seq_file.h (i.e.: uptime.c )
+ 用于减少 /proc文件的体积
1. 实现 seq_operations
    + start/stop/next/show
2. 实现 file_operations 时 使用seq接口
    + open/release/read/write/llseek => seq_read/seq_release/seq_read/seq_write/seq_lseek
+ 最终流程 syscall -> file_operation -> seq_接口 -> seq_operations

# 4 ioctl  ( copy_to_user, copy_from_user )
1. 
2. 
