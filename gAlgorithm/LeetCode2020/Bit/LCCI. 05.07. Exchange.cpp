// Write a program to swap odd and even bits in an integer with as few instructions as possible
// (e.g., bit 0 and bit 1 are swapped, bit 2 and bit 3 are swapped, and so on).

// Input: num = 2（0b10）
// Output 1 (0b01)

// Input: num = 3
// Output: 3

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/exchange-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int exchangeBits(int num) {
        return ((num & 0x55555555) << 1) | ((num & 0xaaaaaaaa) >> 1);
    }
};