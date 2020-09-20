#define min(x, y) (x < y ? x : y)

// 迭代法
int sort_merge(int *a, int len) {
    int *c = a; // 缓存最初的指针; 在归并过程中会丢失
    int *b = new int[len];
    for (int seg = 1; seg < len; seg *= 2) { // seg 指示当前循环下; 小单元待排序数组的长度 / 2
        for (int sub = 0; sub < len; sub += seg*2) {  // sub 循环 小单元待排序数组; 所以 sub 步进为 seg*2
            int low  = sub;
            int mid  = min((sub + seg),   len);
            int high = min((sub + seg*2), len);

            int k  = low;
            int sub1 = low, end1 = mid;
            int sub2 = mid, end2 = high;

            while (sub1 < end1 && sub2 < end2)
                b[k++] = a[sub1] < a[sub2] ? a[sub1++] : a[sub2++];
            
            while (sub1 < end1)
                b[k++] = a[sub1++];
            while (sub2 < end1)
                b[k++] = a[sub2++];
        
            // 单次循环过程中; b[] 存储临时结果
        }
        int *t = a;
        a = b;      // 单次循环结束后; a[] 指示已完成单次循环排序的结果
        b = t;      // 而 b[] 为undefined; 用于下次循环的排序结果
    }
    // 实际上 a; b 指针互为 临时缓存和结果; ping-pong 交换;
    // 因此   最终得把结果存储在 传入指针中
    if (c != a) {   
        for (int i  = 0; i < len; ++i)
            c[i] = a[i];
        b = a; // 用于释放内存
    }
    delete b;

    // seg = 1; sub = 0; >>=>> 待排序子数组{ a[0, 2) }
    // seg = 1; sub = 2; >>=>> 待排序子数组{ a[2, 4) }
    // seg = 1; sub = 4; >>=>> 待排序子数组{ a[4, 6) }
    // seg = 1; sub = 6; >>=>> 待排序子数组{ a[6, 8) }
    // ...
    // seg = 2; sub = 0; >>=>> 待排序子数组{ a[0, 4) }
    // seg = 2; sub = 4; >>=>> 待排序子数组{ a[4, 8) }
}

// 递归法
void __sort_merge(vector<int> &a, int begin, int mid, int end) {
    vector<int> b(end - begin + 1, 0);
    int i = begin, j = mid, k = 0;
    while (begin < mid && j <= end) {
        if (a[i] < a[j])
            b[k++] = a[i++];
        else
            b[k++] = a[j++];
    }
    while (i < mid)
        b[k++] = a[i++];
    while (j <= end)
        b[k++] = a[j++];
    for (int x = 0, y = begin; x < b.size(); ++x, ++y)
        a[y] = b[x];
}

void sort_merge(vector<int> &a, int begin, int end) {
    if (begin >= end) // 1. 确定递归终止条件
        return;
    int mid = (begin + end ) / 2;
    sort_merge(a, begin, mid);
    sort_merge(a, mid + 1, end);
    __sort_merge(a, begin, mid, end);
}