'''
通用logger类:
recommanded:
输出文件指向
每条要用时间
每条分级: Warning, Error, Info
要有Tag
是否输出到 终端

optional:
颜色, 输出到终端才有用,
{
    "red"   : "\033[91m",    # Error
    "green"  : "\033[92m",
    "yellow" : "\033[93m",	 # Warning
    "end"    : "\033[0m",    # Info, 一般颜色
}

'''