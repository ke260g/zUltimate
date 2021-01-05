// You are given two 32-bit numbers, N and M, 
// and two bit positions, i and j.
// Write a method to insert M into N such that 
// M starts at bit j and ends at bit i.
// You can assume that the bits j through i have enough space to
// fit all of M. That is, if M = 10011,
// you can assume that there are at least 5 bits between j and i. 
// You would not, for example, have j = 3 and i = 2, 
// because M could not fully fit between bit 3 and bit 2.

// Input: N = 10000000000, M = 10011, i = 2, j = 6
// Output: N = 10001001100

// Input:  N = 0, M = 11111, i = 0, j = 4
// Output: N = 11111

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/insert-into-bits-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 1. 求出 M 的掩码; 不需要求有效位数; 因为j-i 就是有效位数
// 2. 把 M的掩码 左移i; 使得 N 对应的bit 置为0
// 3. 把 M本身   左移i; 最后或上 N;
class Solution {
public:
    int insertBits(int N, int M, int i, int j) {
        unsigned mask = (1 << (j - i + 1)) - 1;
        N &= ~(mask << i);
        return N | (M << i);
    }
};