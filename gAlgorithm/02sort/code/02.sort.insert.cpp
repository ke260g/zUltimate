#include <vector>
int sort_insert(vector<int> a) {
    for (int i = 1; i < a.size(); ++i) {
        int val = a[i];    // 1. 把改数值插入(左移)到合适的位置
        int j = i - 1;     // 2. j+1 指示 待插入数值的位置 
        while (j >= 0 && val < a[j]) { // 3. 从右往左扫描
            a[j+1] = a[j]; // 4. 左移; 此处 j   即待插入数值的位置
            --j;           // 5. --j; 使得 j+1 即待插入数值的位置
        }
        a[j+1] = val;      // 6.
    }
}