// You have a total of n coins that you want to form in a staircase shape,
// where every k-th row must have exactly k coins.

// Given n, find the total number of full staircase rows that can be formed.

// n is a non-negative integer and fits within the range of a 32-bit signed integer.


// Example 1:
// n = 5
// The coins can form the following rows:
// ¤
// ¤ ¤
// ¤ ¤
// Because the 3rd row is incomplete, we return 2.

// Example 2:
// n = 8
// The coins can form the following rows:
// ¤
// ¤ ¤
// ¤ ¤ ¤
// ¤ ¤
// Because the 4th row is incomplete, we return 3.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/arranging-coins
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 假设 completed 的 staircase shape 有 m 行
// 那么 需要 ((1+m)*m)/2 个 coins (高斯求和公式)
// 因此 求满足 ((1+m)*m)/2 <= n) 的 m 最大值

// 二分法
// 注意溢出; 要用 long long
// 形同 00069 Sqrt(x) 问题
class Solution {
public:
    int arrangeCoins(int n) {
        // (1 + m) * m / 2 <= n;
        // 求 满足条件的 m 的 最大值
        int l = 0, r = n;
        int ans = 0;
        while (l <= r) {
            long long m = l + (r-l)/2;
            if (((1+m)*m)/2 <= n) {
                l = m + 1;
                ans = max(ans, (int)m);
            } else {
                r = m - 1;
            }
        }
        return ans;
    }
};