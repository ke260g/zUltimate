// Given a non-negative integer x, compute and return the square root of x.

// Since the return type is an integer, the decimal digits are truncated, 

// and only the integer part of the result is returned.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/sqrtx
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 求平方根

// 自己的做法, i++ 太慢了
class Solution {
public:
    int mySqrt(int x) {
        unsigned int i = 1;
        // i++
        // 找到一个平方 > x 的 i
        // 则平方根时 i-1
        while (i < INT_MAX) {
            if (i*i > x)
                break;
            ++i;
        }
        return i-1;
    }
};

// 题解a)
// 1. 二分查找法; 起始区间是 [1, x/2], 找到最接近的 i*i <= x 的 i
// 2. 坑1: 左右区间相等时; 也要迭代
// 3. 坑2: mid*mid 小于等于时; 说明需要更新 结果
//         因为这个可能就是最接近的 根
// 4. 坑3: mid 的 溢出问题
// 5. 坑4: x == 1 时; 右区间会变成0; 要提前返回
class Solution {
public:
    int mySqrt(int x) {
        if (x == 1 || x == 0) return x;
        int l = 0, r = x/2, result = -1;
        while (l <= r) {
            long long tmp = (l + r) / 2;
            if (tmp*tmp <= x) {
                result = tmp;
                l = tmp+1;
            } else {
                r = tmp-1;
            }
        }
        return result;
    }
};