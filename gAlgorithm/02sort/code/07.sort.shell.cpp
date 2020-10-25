// https://www.cnblogs.com/chengxiao/p/6104371.html
// https://blog.csdn.net/qq_39207948/article/details/80006224
void sort_shell(vector<int> &a) {
    // 三级循环
    for (int gap = a.size()/2; gap > 0; gap /= 2) { // 按照增量分组; 初始增量是 size/2
        for (int i = gap; i < a.size(); i += gap) { // i 作为分组的末元素; 尾部开始递归
            int j = i;
            int pivot = a[i];
            for (j -= gap; j >= 0; j -= gap) {
                if (a[j] > pivot)
                    a[j + gap] = a[j];
                else
                    break;
            }
            a[j+gap] = pivot;
        }
    }    
}