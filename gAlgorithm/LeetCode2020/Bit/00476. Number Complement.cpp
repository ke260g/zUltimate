// Given a positive integer num, output its complement number. 
// The complement strategy is to flip the bits of its binary representation.

// Input: num = 5
// Output: 2
// Explanation: The binary representation of 5 is 101 (no leading zero bits), 
//              and its complement is 010. So you need to output 2.

// Input: num = 1
// Output: 0
// Explanation: The binary representation of 1 is 1 (no leading zero bits),
//              and its complement is 0. So you need to output 0.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/number-complement
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 二进制的补数

class Solution {
public:
    int findComplement(int num) {
        // 1. 求出有效bit的个数; 即最左边1的位置
        // 2. 求出其掩码
        // 3. 原始数值取反; 掩码取与
        int n = num, nbit = 0;
        while(n){
            ++nbit;
            n >>=1;
        }
        unsigned mask = (((unsigned) 1 << (nbit-1)) - 1);
        return (unsigned)(~num) & mask;
    }
};