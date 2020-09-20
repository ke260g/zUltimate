// https://www.cnblogs.com/litexy/p/9744355.html (算法写得low；pivot选取写得好)
// http://data.biancheng.net/view/117.html       (原理描述很清晰; 代码写得烂; 别看)
// https://blog.csdn.net/Tizzzzzz/article/details/79610375 (排序例子; 没有代码)
int pivotOfFist(vector<int> &a, int first, int last) { return first; }

int pivotOfLast(vector<int> &a, int first, int last) { return last; }

int pivotOfMedian(vector<int> &a, int first, int last) {
    int mid = 0;
    if ((last - first) % 2 == 0) // 元素个数是基数
        mid = (last - first) / 2;
    else
        mid = (last - first - 1) / 2;
    
    // if (x < y < z) or (z < y < x); 
    // then (x - y) * (z - y) < 0
    if ((a[first] - a[mid]) * (a[last] - a[mid]) < 0)
        return mid;
    else if ((a[first] - a[last]) * (a[mid] - a[last]) < 0)
        return last;
    else
        return first;
}

void __sort_quick(vecotr<int> &a, int first, int last) {
    if (last >= first)
        return;
    int pivot = pivotOfMedian(a, first, last);
    int pivot_value = a[pivot];
    if (pivot != first) swap(a[pivot], a[first]); // 基准值放到数组头; 不参与划分
    int i = first + 1, j = last;
    while (i < j) {
        if (a[i] <= pivot) {
            i++;
            continue;
        }
        if (pivot <= a[j]) {
            j--;
            continue;
        }
        swap(a[i], a[j]);
        i++; j--;
    }
    swap(a[first], a[i]); // 最后把 基准值放到数组中间
    __sort_quick(a, first, i - 1);
    __sort_quick(a, i + 1, last);
}

void sort_quick(vecotr<int> &a) {
    if (a.size() < 2)
        return;
    __sort_quick(a, 0, a.size()-1);
}