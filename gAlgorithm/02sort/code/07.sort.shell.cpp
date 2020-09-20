// https://www.cnblogs.com/chengxiao/p/6104371.html
// https://blog.csdn.net/qq_39207948/article/details/80006224
void sort_shell(vector<int> &a) {
    // 三级循环
    for (int gap = a.size() / 2; a > 0; gap /= 2) {  // 按照增量分组; 初始增量是 size/2
        for (int i = gap; i <= a.size(); i += gap) { // i 作为分组的末元素; 因为是插入排序; 所以尾部开始递归
            // pre     指向 当前元素的前驱元素
            // pre+gap 指向 当前元素的最终位置
            int val = a[j];
            int pre = i - gap;
            while (pre >= 0 && val < a[pre]) {          
                a[pre+gap] = val;                      
                pre -= gap;
            }
            a[pre+gap] = val;
        }
    }
}