// https://www.cnblogs.com/skywang12345/p/3603669.html (代码是错的)
// https://blog.csdn.net/u013761665/article/details/51695211

int max(vector<int> &a) {
    int max = a[0];
    for (int i = 1; i < a.size(); ++i)
        max = max(max, a[i]);
}

inline int value_on_pos(int value, int pos) { // 数位上的值
    return (value / exp) % 10;
}

void __sort_radix(vector<int> &a, int exp) {
    // 1. 分配桶
    vector<int> b(a.size(), 0); // 存储"被排序数据"的临时数组
    vector<int> counter(10, 0); // 计数器
    // 2. 将数据出现的次数存储在counter[]中
    for (int i = 0; i < a.size(); i++) 
        counter[ value_on_pos(a[i], exp) ]++;

    // 3. 变换 counter 为 位置数组;  counter[i] 指示 其代表的数值 在排序后最后出现的索引
    for (int i = 1; i < counter.size(); i++)
        counter[i] += counter[i - 1];
    
    // 4. 倒序遍历原始数组，从统计数组找到正确位置，输出到结果数组
    for (int i = a.size() - 1; i >= 0; i--)
        b[--counter[value_on_pos(a[i], exp)]] = a[i];
        /* 因为 counter[i] 是从1开始算的, 所以索引必须减一 */

    // 5. 将排序好的数据赋值给a[]
    for (int i = 0; i < a.size(); i++)      
        a[i] = b[i];
}
void sort_radix(vector<int> &a) {
    int max = max(a);
    for (int exp = 1 /* 指数表示数位 */; max / exp > 0; exp *= 10 /* 10进制 */)
        sort_count(a, exp);
}