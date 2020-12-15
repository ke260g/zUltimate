// https://www.cnblogs.com/kyoner/p/10604781.html
// https://www.jianshu.com/p/86c2375246d7
void sort_count(vector<int> &a) {
    // 1. 分配计数数组
    int min_value = a[0], max_value = a[0];
    if (a.size() < 2)
        return;
    for (int i = 1; i < a.size(); i++)  {
        min = min(min, a[i]); // 找到最小值
        max = max(max, a[i]); // 找到最大值
    }    
    vector<int> count_array(max - min + 1, 0);

    // 2. 计算出现次数
    for (int i = 0; i < a.size(); i++) { 
        count_array[a[i] - min]++;
    }

    // 3. 变换为位置数组 ==> 当前索引所代表的数字在 在排序后最后出现的索引
    for (int i = 1; i < count_array.size(); i++) // 后面的元素的位置 等于前面的元素位置之和
        count_array[i] += count_array[i-1]; // 当然; 原本计数为0的位置上也会变得有数值 用以传递计数

    //4.倒序遍历原始数组，从统计数组找到正确位置，输出到结果数组
    vector<int> b (count_array.size(), 0);
    for (int i = a.size() - 1; i > 0; i--) {
        b[ --count_array[a[i] - min]/* 所在位置 */]  = a[i];
        /* 因为 count_array[i] 是从1开始算的, 所以索引必须减一 */
    }

    // 5. 将排序好的数据赋值给a[]
    a.assign(b.begin(), b.end());
}