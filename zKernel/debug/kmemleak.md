占用额外的内存; 影响内核分配内存的速度;
如果有泄漏, 会生成kmemleak文件

内核参数
CONFIG_HAVE_DEBUG_KMEMLEAK=y
CONFIG_DEBUG_KMEMLEAK=y
CONFIG_DEBUG_KMEMLEAK_DEFAULT_OFF=y
CONFIG_DEBUG_KMEMLEAK_EARLY_LOG_SIZE=5000

To trigger an intermediate memory scan
`echo scan > /sys/kernel/debug/kmemleak`

To clear the list of all current possible memory leaks
`echo clear > /sys/kernel/debug/kmemleak`

To disable
`echo off > /sys/kernel/debug/kmemleak`

全部的kmemleak参数
off
    disable kmemleak (irreversible)
stack=on
    enable the task stacks scanning (default)
stack=off
    disable the tasks stacks scanning
scan=on
    start the automatic memory scanning thread (default)
scan=off
    stop the automatic memory scanning thread
scan=<secs>
    set the automatic memory scanning period in seconds (default 600, 0 to stop the automatic scanning)
scan
    trigger a memory scan
clear
    clear list of current memory leak suspects, done by marking all current reported unreferenced objects grey, or free all kmemleak objects if kmemleak has been disabled.
dump=<addr>
    dump information about the object found at <addr>

