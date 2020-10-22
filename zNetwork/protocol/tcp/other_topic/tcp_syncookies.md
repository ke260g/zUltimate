[TOC]
https://segmentfault.com/a/1190000019292140 原理
https://blog.csdn.net/sinat_20184565/article/details/104828782 实现
https://lwn.net/Articles/277146/ 设计

内核实现函数调用栈
```c++
tcp_conn_request()
    > cookie_init_sequence
    > cookie_v4_init_sequence
    > __cookie_v4_init_sequence
    > secure_tcp_syn_cookie
```