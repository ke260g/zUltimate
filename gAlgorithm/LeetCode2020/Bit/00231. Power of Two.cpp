// Given an integer n, return true if it is a power of two. Otherwise, return false.

// An integer n is a power of two, if there exists an integer x such that n == 2^x.

// Input: n = 1
// Output: true
// Explanation: 20 = 1

// Input: n = 16
// Output: true
// Explanation: 24 = 16

// 判断是否是 2 的指数幂
// 即判断 x 的二进制表达是否有且只有一个1

// 方法1
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if (n == 0) return false;
        // 定律1: x & (-x) 求得最右边的1
        // 因为负数的表示法 x == ~x + 1
        // x取反后 +1; 会往前进位; 直到变成x右边第一个1
        // 所以，如果 x 有且只有一个1, 那么 x&(-x) == x
        long x = n;
        return (x & (-x)) == x;
    }
};

// 方法2: (使用 00191. Number of 1 Bits 干掉最右边的1的方法)
class Solution {
public:
    bool isPowerOfTwo(int n) {
        // 定律2: x & (x-1) 可以干掉最右边的1
        // 1. 假如 x 最右边的1右边有 p 个 0
        // 2. 那么 x-1后; 因为减法借位; 最右边的1置零; 
        //    右边 p 个都变成1
        // 3. 所以 x & (x-1) 干掉最右边的1
        //
        //
        // 如果 x 有且只有1个1; 那么 x & (x-1) == 0
        //
        // 又因为 2的指数幂必须是正数; x > 0
        return n > 0 && (n & (n-1)) == 0;
    }
};
