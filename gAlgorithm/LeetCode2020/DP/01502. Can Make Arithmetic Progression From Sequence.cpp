#include<algorithm>
class Solution {
public:
    // 等差数列
    // 1. 排序
    // 2. 数列长度小于 3 的; 按等差数列处理
    // 3. 数列长度大于 3 的; a[i-1]*2 == a[i] + a[i-2] (2<= i <= size)
    bool canMakeArithmeticProgression(vector<int>& arr) {
        if (arr.size() < 3) return true;
        sort(arr.begin(), arr.end());
        for (int i = 2; i < arr.size(); ++i) {
            if (arr[i-2] + arr[i] != arr[i-1]*2)
                return false;
        }
        return true;
        
    }
};