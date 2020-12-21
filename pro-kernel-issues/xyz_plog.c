/**
 * @file 把 log 打印到 /proc
 *       在初始化时指定 "总行数" 和 "行大小"
 *       需要较大的内存 2*总行数*行大小
 *       日志有两个buff, 一个满了就会使用另一个
 * @note 不支持重置 "总行数" 和 "行大小"
 *       日志函数没有加锁, 多cpu场景下会出问题
 */
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <stdarg.h>

extern struct proc_dir_entry *xyz_proc_root;

#define xyz_plog_line (128)
#define xyz_plog_size (4096)

static char bufu[xyz_plog_line*xyz_plog_size];
static char bufv[xyz_plog_line*xyz_plog_size];

static struct xyz_plog_t{
    const uint32_t line; // 行长度
    const uint32_t size; // 总行数
    char *const buff[2]; // 两个 buff
    uint32_t curr; // 当前行数
    uint32_t bufp; // 当前 buff 索引
    uint32_t swap; // 标记是否 swap
    uint32_t init; // 标记是否 init
} plog = {
    .line = 128,
    .size = 4096,
    .buff = { bufu, bufv },
};

#define plog_bufp_next(bufp) ((!bufp) & 0x1)

/**
 * @brief 检查内存空间, 是否满了, 满了就交换
 * 
 */
static void xyz_plog_swap() {
    if (plog.curr < plog.size)
        return;
    plog.swap = 1;
    plog.bufp = plog_bufp_next(plog.bufp);
    memset(plog.buff[plog.bufp], 0, plog.line*plog.size);
    plog.curr = 0;
}

static int xyz_proc_plog_show(struct seq_file *m, void *v) {
    int i;
    if (plog.swap) {
        int next = plog_bufp_next(plog.bufp);
        for (i = 0; i < plog.size; ++i)
            seq_printf(m, "%s\n", plog.buff[next] + plog.line*i);
    }
    for (i = 0; i < plog.curr; ++i)
        seq_printf(m, "%s\n", plog.buff[plog.bufp] + plog.line*i);
    return 0;
}

static int xyz_proc_plog_open(struct inode * inode, struct file * file) {
    return single_open(file, xyz_proc_plog_show, NULL);
}

static struct file_operations xyz_proc_plog_file_ops = {
    owner:      THIS_MODULE,
    open:       xyz_proc_plog_open,
    read:       seq_read,
    llseek:     seq_lseek,
    release:    single_release,
};

int xyz_plog_create() {
    struct proc_dir_entry *entry;
    PROC_CREATE(entry, "plog", 0666, xyz_proc_root, &xyz_proc_plog_file_ops);
    if (entry == NULL)
        return -1;
    plog.init = 1;
    return 0;
}

 __attribute__((format(printf, 1, 2))) int xyz_plog_printf(const char *format, ...) {
    char *line;
    va_list ap;

    if (!plog.init) return -1;

    xyz_plog_swap();
    line = plog.buff[plog.bufp] + plog.line * plog.curr;

    va_start(ap, format);
    vsnprintf(line, plog.line, format, ap); 
	va_end(ap);

    ++plog.curr;
    return 0;
}

int xyz_plog_remove() {
    remove_proc_entry("plog", xyz_proc_root);
    plog.init = 0;
    return 0;
}