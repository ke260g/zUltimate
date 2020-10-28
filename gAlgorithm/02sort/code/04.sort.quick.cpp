// https://www.cnblogs.com/litexy/p/9744355.html (算法写得low；pivot选取写得好)
// http://data.biancheng.net/view/117.html       (原理描述很清晰; 代码写得烂; 别看)
// https://blog.csdn.net/Tizzzzzz/article/details/79610375 (排序例子; 没有代码)
int pivotOfFist(vector<int> &a, int head, int tail) { return first; }

int pivotOfLast(vector<int> &a, int head, int tail) { return last; }

int pivotOfMedian(vector<int> &a, int head, int tail) {
    int mid = (tail - head) % 2 ?  (tail + head - 1)/2 : (tail + head) / 2;
    if ( (a[mid] - a[head])*(a[tail]-a[head]) < 0)
        return head;
    if ( (a[mid] - a[tail])*(a[head]-a[tail]) < 0)
        return tail;
    return mid;
}

void __sort_quick(vecotr<int> &a, int head, int tail) {
    if (head >= tail) return;
    int pivot_index = findpivot(a, head, tail); // 获取基准值
    int pivot = a[pivot_index];
    swap(a[pivot_index], a[head]); // 基准值放到数组头; 不参与划分
#if 1  // 双指针法::滑动窗口

    int mid = head+1, i = head+1;
    // [head+1, mid) // < pivot
    // [mid, i)      // >= pivot (等于 pivot; 必须放到窗口内)
    // [i, tail]     // pending
    for (int i = head+1; i <= tail; ++i) {
        if (a[i] < pivot)
            swap(a[i], a[mid++]);
    }
    // 排序后:
    //   [head+1, mid) 是 <  pivot 的
    //   [mid, tail]   是 >= pivot
    // 所以先把 pivot 放中间; 然后进行分割
    swap(a[head], a[mid-1]);
    __sort_quick(a, head, mid-2);
    __sort_quick(a, mid, tail);
#else // 双指针法::左右对称
    int i = head, j = tail;
    while(i < j) {
        // 必须从后面开始走起;
        // 因为 pivot 在head这里; 第一次的时侯
        // 前面首成员已经为 候选交换成员
        // 所以要找到后面的 候选交换成员
        while(a[j] > pivot && i < j)
            --j;
        if (i < j)
            a[i++] = a[j];

        while(a[i] < pivot && i < j)
            ++i;
        if (i < j)
            a[j--] = a[i];
    }
    a[i] = pivot; // 把基准值还回去
    __sort_quick(a, head, i-1);
    __sort_quick(a, i+1, tail);
#endif
}

void sort_quick(vecotr<int> &a) {
    if (a.size() < 2)
        return;
    __sort_quick(a, 0, a.size()-1);
}