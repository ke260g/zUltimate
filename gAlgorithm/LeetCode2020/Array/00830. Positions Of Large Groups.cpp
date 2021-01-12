/*
 * @lc app=leetcode id=830 lang=cpp
 *
 * [830] Positions of Large Groups
 *
 * https://leetcode.com/problems/positions-of-large-groups/description/
 *
 * algorithms
 * Easy (50.17%)
 * Total Accepted:    51.6K
 * Total Submissions: 102.8K
 * Testcase Example:  '"abbxxxxzzy"'
 *
 * In a string s of lowercase letters, these letters form consecutive groups of
 * the same character.
 * 
 * For example, a string like s = "abbxxxxzyy" has the groups "a", "bb",
 * "xxxx", "z", and "yy".
 * 
 * A group is identified by an interval [start, end], where start and end
 * denote the start and end indices (inclusive) of the group. In the above
 * example, "xxxx" has the interval [3,6].
 * 
 * A group is considered large if it has 3 or more characters.
 * 
 * Return the intervals of every large group sorted in increasing order by
 * start index.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "abbxxxxzzy"
 * Output: [[3,6]]
 * Explanation: "xxxx" is the only large group with start index 3 and end index
 * 6.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "abc"
 * Output: []
 * Explanation: We have groups "a", "b", and "c", none of which are large
 * groups.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "abcdddeeeeaabbbcd"
 * Output: [[3,5],[6,9],[12,14]]
 * Explanation: The large groups are "ddd", "eeee", and "bbb".
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: s = "aba"
 * Output: []
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 1000
 * s contains lower-case English letters only.
 * 
 * 
 */

// 1. 遍历数组
// 2. 当 group-strat-index  < 0；那么当前索引称为开头
// 3. 当连续字符末尾时 (即 s[i] != s[i+1])
// 3.1 通过之前记录的 group-strat-index 计算 group 大小
//     即 i - start + 1; 满足条件则 group 入列
// 3.2 重置 group-strat-index
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string s) {
        vector<vector<int>> ans;
        if (s.empty()) return ans;
        vector<int> group(2, 0);
        int start = -1;
        for (int i = 0; i < s.size(); ++i) {
            if (start < 0)
                start = i;
            if (i+1 == s.size() || s[i] != s[i+1]) {
                if (i - start + 1 >= 3) {
                    group[0] = start;
                    group[1] = i;
                    ans.push_back(group);
                }
                start = -1;
            }
        }
        return ans;
    }
};