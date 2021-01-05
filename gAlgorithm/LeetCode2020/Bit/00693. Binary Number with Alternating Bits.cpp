// Given a positive integer, check whether it has alternating bits: namely, 
// if two adjacent bits will always have different values.

// Input: n = 5
// Output: true
// Explanation: The binary representation of 5 is: 101

// Input: n = 7
// Output: false
// Explanation: The binary representation of 7 is: 111.

// Input: n = 11
// Output: false
// Explanation: The binary representation of 11 is: 1011.

// Input: n = 10
// Output: true
// Explanation: The binary representation of 10 is: 1010.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/binary-number-with-alternating-bits
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
class Solution {
public:
    bool hasAlternatingBits(int n) {
        // 1. 右移一位再xor; 
        //   如果n满足 10101... ; 那么错位xor 必然右边都是1
        // 2. 把 错位xor 结果 +1; 求得 1000000 这样的2指数幂
        // 3. 用 00231. Power of Two 的方法; 判断是否是2指数幂
        long x = n;
        x = (x ^ (x >> 1)) + 1;
        return (x & (x - 1)) == 0;
    }
};