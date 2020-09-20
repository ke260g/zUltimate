// https://www.cnblogs.com/chengxiao/p/6129630.html
// https://www.cnblogs.com/skywang12345/p/3602162.html
void __sort_heap(vector<int> &a, int idx) {
    int val = a[idx];
    for (int i = 2*idx + 1; i < a.size(); i += 2*i + 1 /* 下一个左节点 */ ) {
        if (i+1 < a.size() && a[i+1] > a[i])  /* 找到 较大子节点 */
            i++;
        if (val < a[i]) {    /* 当前节点 小于 较大子节点交换; 不是真的交换 */
            a[idx] = a[i];   /* 先把 "较大子节点" 放到当前节点 */ 
            idx = i;         /* 更新 index 指向 "当前节点" 的位置, 遍历结束后才真的交换 */
        } else               /* 满足条件; 终止循环 */
            break;
    }
    a[idx] = val;            /* 遍历结束后才真的交换 */
}


void sort_heap(vector<int> &a) {
    if (a.size() < 2）
        return;
    for (int i = a.size() / 2 - 1; i >= 0; i--)
        __sort_heap(a, i)
    for (int i = a.size() - 1; i >= 0; i--) {
        swap(a[0], a[i]);
        __sort_heap(a, 0);
    }
}