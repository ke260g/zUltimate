// Write a function that adds two numbers.
// You should not use + or any arithmetic operators.

// Input: a = 1, b = 1
// Output: 2

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/add-without-plus-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
class Solution {
public:
    int add(int a, int b) {
        while (b) {
            unsigned c = (unsigned)(a & b) << 1;
            a = (a ^ b);
            b = c;
        }
        return a;
    }
};