// Calculate the sum of two integers a and b, but you are not allowed to use the operator + and -
// 位运算实现加法
// [M] xor 实际上是 加法不计算进位 1^1=0, 0^0=0, 1^0=1
// [N] and 实际上是 加法求进位位值 (x & y) << 1
// 因此反复进行 [M] + [N] 即可; 直到 N 不存在进位, 即 N == 0

class Solution {
public:
    int getSum(int a, int b) {
        while (b != 0) {
            unsigned c = (unsigned)(a & b) << 1; // 计算进位
            a = a ^ b;               // 计算无进位的结果 > 1^1=0, 0^0=0, 1^0=1
            b = c;                   // 加上进位
        }
        return a;
    }
};
