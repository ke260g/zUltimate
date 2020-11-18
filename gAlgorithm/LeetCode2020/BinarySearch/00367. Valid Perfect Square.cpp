// Given a positive integer num, write a function which returns True if num is a perfect square else False.

// Follow up: Do not use any built-in library function such as sqrt.

// Input: num = 16
// Output: true

// Input: num = 14
// Output: false

// 判断一个数是否是平方数
// 在区间 [0, num] 用二分查找
// 找到的条件是 m*m == num
class Solution {
public:
    bool isPerfectSquare(int num) {
        if (num == 0 || num == 1) return true;
        int l = 1, r = num/2;
        while (l <= r) {
            long long m = l + (r-l)/2;
            if (m*m == num) {
                return true;
            } else if (m*m < num) {
                l = m+1;
            } else if (m*m > num) {
                r = m-1;
            }
        }
        return false;
    }
};