// https://www.cnblogs.com/chengxiao/p/6129630.html
void __sort_heap(vector<int> &a, int idx) {
    // 1. 逐层孩子开始调整
    for (int i = 2*n+1/*左孩*/; i < size; i = 2*i + 1) {
        // 2. 从两个孩子找到较大的一个
        if (i+1 < size && a[i+1] > a[i])
            i++;
        // 3. 当前节点大于两个孩子; 符合条件; 调整结束
        if (a[n] >= a[i])
            break;
        // 4. 当前节点于较大的孩子交换;
        //    之前 2. 步骤中已经找到较大孩子了
        swap(a[n], a[i]);
        // 5. 当前节点 指向调整后的节点索引
        n = i;
    }
}

void sort_heap(vector<int> &a) {
    if (a.size() < 2) return;
    // 1. 从末尾节点的父节点开始; 从后往前逐个节点调整
    for (int i = a.size()/2 - 1; i >= 0; i--)
        __sort(a, i, a.size());
    // 2. 把头节点于尾节点交换; 交换后调整
    for (int i = a.size()-1; i > 0; i--) {
        swap(a[0], a[i]);
        __sort(a, 0, i);
    }
}