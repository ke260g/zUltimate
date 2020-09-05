[TOC]
# overview
man 7 sysvipc
man 7 sem_overview



man 7 ftok

```c++
#include <sys/ipc.h>
key_t ftok(const char *pathname, int proj_id);  // 输入pathname; 输出 ipc 的 key (uuid)
```