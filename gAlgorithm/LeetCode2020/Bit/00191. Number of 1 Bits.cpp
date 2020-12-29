// Write a function that takes an unsigned integer and 
// returns the number of '1' bits it has 
// (also known as the Hamming weight).

// Input: n = 00000000000000000000000000001011
// Output: 3
// Explanation: The input binary string 00000000000000000000000000001011 has a total of three '1' bits.

// Input: n = 00000000000000000000000010000000
// Output: 1
// Explanation: The input binary string 00000000000000000000000010000000 has a total of one '1' bit.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/number-of-1-bits
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 数学术语称为 汉明重量
// 00461 汉明距离 是 同类问题集合

// 自己想的 原始方法: 遍历32bit;
// & 0x1 计算1的个数
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int sum = 0;
        while (n) {
            sum += n & 0x1;
            n >>= 1;
        }
        return sum;
    }
};

// 题解的优化算法
// n &= n - 1; 实际上该运算每次迭代 会 减少1个bit
// 每次都会干掉最右边的一个1
// 这使得 算法复杂度 一定程度上 降低了; 不再是恒定的 32 次
class Solution {
public:
    int hammingWeight(uint32_t n) {
        int sum = 0;
        while (n) {
            sum++;
            n &= n - 1;
        }
        return sum;
    }
};