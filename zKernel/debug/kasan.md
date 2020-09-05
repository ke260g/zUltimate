[TOC]
入门 http://www.wowotech.net/memory_management/424.html

KASAN是一个动态检测内存错误的工具。
KASAN可以检测全局变量、栈、堆分配的内存发生越界访问等问题。
功能比SLUB DEBUG齐全并且支持实时检测。
CONFIG_KASAN=y
