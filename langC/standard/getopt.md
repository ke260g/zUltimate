

# getopt getopt_long getsubopt
```c++
/**
 * @brief getopt 根据 @optstring 解析 argv[optind], optind 进程初始化为1
 *        解析结果返回到 @optopt 和 @optarg
 */
int getopt(int argc, char * const argv[], const char *optstring);
/**
 * @longopts: 数组的最后一个成员必须用填NULL
 * @longinex: 表示当前 longopts[longindex];
 */
int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);
/**
 * @has_arg: 
 */
struct option {
    const char *name;
    int         has_arg; // 0: non-argument; 1: required-argument 2: optional-argument
    int        *flag;    // 传出, argument result or NULL 表示没有结果;
    int         val;     // 
};


extern char *optarg; // 全局变量; 用以返回 参数
extern int optind,   // 全局变量; 用以指示下个要处理的 argv[optind]
           opterr,   // 错误码
           optopt;   // 操作码
// 一般地使用方法
while (opt = getopt(argc, argv, "ab:c::") != -1) {
    switch (opt) {
        case 'a': // a non-argument option
            break;
        case 'b': // a required-argument option
            argument = optarg;
            break;
        case 'c'  // an optional-argument option
            if (optarg) argument = optarg; 
            break;
        case '?'
            print_usage();
            exit(EXIT_FAILURE);
    }
}
while (getopt_long())
```
1. optstring 选项指示
    1. 大写 'W' 是保留字符, 有特殊含义
    2. 
2. optstring 模式指示 (开头字符)
    1. '+' 模式, 一旦遇到首个非 option 的 argv, 马上返回
    2. '-' 遍历 argv 指导错误 or 结束 or getopt遇到'--'提前结束
    3. '--' 用于 getopt_long, 指示长选项
3. 其实本质上 optarg optind optpot 都可以设计为 in/out 参数而不是全局参数