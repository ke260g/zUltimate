ARP缓存表 设计介绍
http://www.cnitblog.com/flutist1225/articles/19996.html

gc_stale_time 决定检查一次相邻层记录的有效性的周期。
gc_interval 垃圾回收间隔时间
gc_thresh1 ARP缓存中的最少层数，如果少于这个数，垃圾收集器将不会运行
gc_thresh2 arp缓存中的最多的记录软限制。垃圾收集器在开始收集前，允许记录数超过这个数字5秒。
gc_thresh3 允许拥有的邻居数量的上限; 
           一旦超过这个上限，并且表中没有可以清理掉的垃圾邻居，那么就无法创建新的邻居