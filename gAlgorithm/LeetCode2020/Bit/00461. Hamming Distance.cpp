// The Hamming distance between two integers is the number of positions at which the 
// corresponding bits are different.

// Given two integers x and y, calculate the Hamming distance.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/hamming-distance
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 汉明距离
// 求出 两个数; 其不同的 bit 位个数
// 1. 取 xor ; 其中1的位置就是 不同的位置
// 2. 求上一步骤结果中地 1 的个数 (00191. Number of 1 Bits)
class Solution {
public:
    int hammingDistance(int x, int y) {
        int n = x^y;
        int d = 0;
        while (n) {
            n &= (n - 1);
            d++;
        }
        return d;
    }
};