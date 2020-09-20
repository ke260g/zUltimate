// https://www.jianshu.com/p/204ed43aec0c              (代码没有考虑负数)
// https://www.cnblogs.com/skywang12345/p/3602737.html (代码没有考虑负数)
int max(vector<int> &a) {
    int max = 0;
    for (int i = 0; i < a.size(); i++)
        max = max(max, a[i]);
    return max;
}

int min(vector<int> &a) {
    int min = 0;
    for (int i = 0; i < a.size(); i++)
        min = min(min, a[i]);
    return min;
}

void bucketSort(vector<int> &a) {
    if (a.size() < 2)
        return;
    
    int max = max(a); // 求出最大值
    int min = min(a); // 求得最小值

    vector<int> buckets(max - min + 1, 0); // 申请内存
    for (int i = 0; i < a.size(); i++)     // 计数
        buckets[a[i] - min]++;
    for (int i = 0, j = 0; i < buckets.size(); i++) {  // 遍历计数数组的每个数字
        while ((buckets[i]--) > 0) // 数字在原始数组a中出现的次数
            a[j++] = i + min;    // 计数数组的索引 + min 即为其表示的原始数值
    }
    return;
}