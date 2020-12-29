// Given an integer array arr. 
// You have to sort the integers in the array in ascending order 
// by the number of 1's in their binary representation and 
// in case of two or more integers have the same number of 1's 
// you have to sort them in ascending order.

// Return the sorted array.

// Input: arr = [0,1,2,3,4,5,6,7,8]
// Output: [0,1,2,4,8,3,5,6,7]
// Explantion: [0] is the only integer with 0 bits.
// [1,2,4,8] all have 1 bit.
// [3,5,6] have 2 bits.
// [7] has 3 bits.
// The sorted array by bits is [0,1,2,4,8,3,5,6,7]

// Input: arr = [1024,512,256,128,64,32,16,8,4,2,1]
// Output: [1,2,4,8,16,32,64,128,256,512,1024]
// Explantion: All integers have 1 bit in the binary representation, you should just sort them in ascending order.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/sort-integers-by-the-number-of-1-bits
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 1. 构造数组 [<原数组成员, bit计数>]
// 2. 排序上一步骤构造地数组
//    a) bit计数相等时; 用原数值排序
//    b) bit计算不相等; 用bit计数排序
// 3. 排序后; 返回新的数组 [<原数组成员>]
class Solution {
public:
    int bitcnt(int n) {
        int cnt = 0;
        while (n) {
            n &= (n - 1);
            cnt++;
        }
        return cnt;
    }
    static bool bitcntcmp(pair<int, int> &x, pair<int, int> &y) {
        return x.second != y.second ? 
               x.second  < y.second :
               x.first   < y.first;
    }
    vector<int> sortByBits(vector<int>& arr) {
        vector<pair<int, int>> a;
        for (auto n: arr)
            a.push_back(make_pair(n, bitcnt(n)));

        sort(a.begin(), a.end(), bitcntcmp);
        vector<int> b(a.size(), 0);
        for (int i = 0; i < a.size(); i++)
            b[i] = a[i].first;
        return b;
    }
};