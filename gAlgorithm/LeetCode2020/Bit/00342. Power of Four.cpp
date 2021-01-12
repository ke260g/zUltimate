// Input: n = 16
// Output: true

// Input: n = 5
// Output: false

// 求 4 的指数
// 1. 先求 2 的指数; (00231. Power of Two)
// 2. 再者 4 的指数其剩余地1个1bits 在奇数位置
//    所以 & 1010 1010 1010 == 0
class Solution {
public:
    bool isPowerOfFour(int n) {
        // 实现1
        return (n > 0) && (n & (n-1)) == 0 && (n & 0xaaaaaaaa) == 0;
        // 实现2:
        return n > 0 && ((n & (n - 1)) == 0) && n & 0x55555555;
    }
};

// 超级解法
// 数学原理: 4 的指数 满足两个条件
// 1. 2 的指数
// 2. 3 的倍数 + 1
// 
// 证明第二条
// 4^n = 4^(n-1) * 4 = 4^(n-1)*3 + 4^(n-1)         <---------- 式子1
//     = (4^(n-2) * 4)*3 + 4^(n-2)*4
//     = (3*4^(n-2) + 4^(n-2))*3 + 3*4^(n-2) + 4^(n-2)
//     = 3*3 * 4(n-2) + 3*2 * 4^(n-2) + 4^(n-2)
//     = 3*(3 + 2) * 4(n-2) + 4^(n-2)              <---------- 式子2
// 一直递推; 直到 右边第二项为 4^(n-n)
class Solution {
public:
    bool isPowerOfFour(int n) {
        return (n > 0) && (n & (n-1)) == 0 && n % 3 == 1;
    }
};