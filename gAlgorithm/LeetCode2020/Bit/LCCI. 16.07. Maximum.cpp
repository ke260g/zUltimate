// Write a method that finds the maximum of two numbers. 
// You should not use if-else or any other comparison operator.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/maximum-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 不使用 比较; if-else 求两个数的最大值
// 可以求两数的差是否是正数z; z用布尔值替代;
// z == 1 表示正数
// z == 0 表示负数
// 那么最大值 = z*a + (!z) * b
//
// 而实际上
// 如果 a - b 是负数; 那么二进制表示器最高位是1
// 如果 a - b 是正数; 那么二进制表示其最高为是0
// 所以还得取一次 ! 布尔值取反求得目标公式
class Solution {
public:
    int maximum(int a, int b) {
        long x = a, y = b;
        long k = !((x - y) >> 63);
        return (k)*a + (!k)*b;
    }
};