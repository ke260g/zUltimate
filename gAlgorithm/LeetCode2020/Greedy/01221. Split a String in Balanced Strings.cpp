// Balanced strings are those who have equal quantity of 'L' and 'R' characters.

// Given a balanced string s split it in the maximum amount of balanced strings.

// Return the maximum amount of splitted balanced strings.

// Input: s = "RLRRLLRLRL"
// Output: 4
// Explanation: s can be split into "RL", "RRLL", "RL", "RL", 
// each substring contains same number of 'L' and 'R'.

// Input: s = "RLLLLRRRLR"
// Output: 3
// Explanation: s can be split into "RL", "LLLRRR", "LR",
// each substring contains same number of 'L' and 'R'.



// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/split-a-string-in-balanced-strings
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 分割平衡字符串
class Solution {
public:
    int balancedStringSplit(string s) {
        int nr = 0, nl = 0, ans = 0;
        // 1. 遍历字符串
        // 2. 分别计算 'R' 和 'L' 的出现次数
        // 3. 当出现次数相等; 则分割; 分割后计数置零
        for (auto c: s) {
            if (c == 'R')
                nr++;
            else if (c == 'L')
                nl++;
            if (nr == nl) {
                nr = nl = 0;
                ans++;
            }
        }
        return ans;
    }
};