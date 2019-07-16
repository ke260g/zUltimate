# man 5 proc
mount -t proc proc /proc

Most of the files in the proc filesystem are read-only, 
but some files are writable, allowing kernel variables to be changed.

**dumpable**
`/proc/[pid]/attr/`   security modules, files are writable.

`/proc/[pid]/cmdline` complete command line
`proc/[pid]/coredump_filter` 
`/proc/[pid]/cwd`     symbolic to current working directory of process
`/proc/[pid]/exe`     symbolic to the actual pathname of the executed command
fd
`/proc/[pid]/fd/`
`/proc/[pid]/fdinfo/`
`/proc/[pid]/io`      high level perspective statistic of io
`/proc/[pid]/maps`    mapped memory regions and their access permissions, always use with debug/valgrind

`/proc/[pid]/mem`     process's memory, readonly, open/read/lseek
`/proc/[pid]/net`
`proc/[pid]/pagemap`  the mapping of each of the process's virtual pages into physical page frames or swap area.
`/proc/[pid]/smaps`   memory consumption for each of the process's mappings `pmap $pid`
`/proc/[pid]/stat`    status `ps`, most common state { R:Running S:Sleeping D:Disk Z:Zombie T:Stopped }
`/proc/[pid]/statm`   memory usage
`/proc/[pid]/status`  human-readable format of stat and statm
`/proc/[pid]/syscall` just address, not function
`/proc/[pid]/timers`  posix timer
`/proc/[pid]/wchan`   当前进程所在的内核函数, 近似系统调用
`/proc/[pid]/task/[tid]/` thread info
`/proc/bus`           hardware bus
`/proc/cmdline`       kernel arguments at boot time
`/proc/cpuinfo`
`/proc/diskstats`
`/proc/filesystems`   filesystems which are supported by the kernel
`/proc/locks`         This file shows current file locks (flock(2) and fcntl(2)) and leases (fcntl(2)).
                      But it can get the filename.
`/proc/meminfo`       memory usage of whole system
`/proc/modules`       `lsmod`
`/proc/net`
`/proc/net/arp`       readable arp
`/proc/net/dev`      
`/proc/net/igmp`
`/proc/net/raw`       raw **socket table**
`/proc/net/snmp`
`/proc/net/tcp`       tcp **socket table**
`/proc/net/udp`       udp **socket table**
`/proc/net/unix`      unix **socket table**
`/proc/scsi`
`/proc/slabinfo`
`/proc/stat`          kernel system status
`/proc/sys`           kernel variable, RDWR, used for setting kernel
`/proc/sysvipc/{ msg, sem, shm }` System V Interprocess Communication (IPC) `man 1 ipcs` `man 7 svipc`
`/proc/vmstat`        various virtual memory statistics
`/proc/version`       `uname -a`

## related
man 2 prctl