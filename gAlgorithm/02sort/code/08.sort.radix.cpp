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
    vector<int> b(a.size(), 0); // 存储"被排序数据"的临时数组
    vector<int> buckets(10, 0); // 桶

    for (int i = 0; i < a.size(); i++) // 将数据出现的次数存储在buckets[]中
        buckets[ value_on_pos(a[i], exp) ]++;

    // 变换 buckets 为 位置数组;  buckets[i] 指示 其代表的数值 在排序后最后出现的索引
    for (int i = 1; i < 10; i++)
        buckets[i] += buckets[i - 1];
    
    for (int i = a.size() - 1; i >= 0; i--) // 将数据存储到临时数组中
        b[--buckets[value_on_pos(a[i], exp)]] = a[i];
        /* 因为 buckets[i] 是从1开始算的, 所以索引必须减一 */

    for (int i = 0; i < a.size(); i++)      // 将排序好的数据赋值给a[]
        a[i] = b[i];
}
void sort_radix(vector<int> &a) {
    int max = max(a);
    for (int exp = 1 /* 指数表示数位 */; max / exp > 0; exp *= 10 /* 10进制 */)
        sort_count(a, exp);
}