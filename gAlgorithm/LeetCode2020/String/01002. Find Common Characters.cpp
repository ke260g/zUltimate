/*
 * @lc app=leetcode id=1002 lang=cpp
 *
 * [1002] Find Common Characters
 *
 * https://leetcode.com/problems/find-common-characters/description/
 *
 * algorithms
 * Easy (68.03%)
 * Total Accepted:    91.8K
 * Total Submissions: 134.8K
 * Testcase Example:  '["bella","label","roller"]'
 *
 * Given an array A of strings made only from lowercase letters, return a list
 * of all characters that show up in all strings within the list (including
 * duplicates).  For example, if a character occurs 3 times in all strings but
 * not 4 times, you need to include that character three times in the final
 * answer.
 * 
 * You may return the answer in any order.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: ["bella","label","roller"]
 * Output: ["e","l","l"]
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: ["cool","lock","cook"]
 * Output: ["c","o"]
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * 1 <= A.length <= 100
 * 1 <= A[i].length <= 100
 * A[i][j] is a lowercase letter
 * 
 * 
 * 
 */
class Solution {
public:
    vector<string> commonChars(vector<string>& A) {
        // 1. 遍历每个字符串 的每个字符
        //    求得每个字符串-字母的频率
        // 2. 遍历每个字符串-字母的频率 的每个字母
        //    求每个字母的最小出现次数
        // 3. 如果 > 0; 则把该字母的最小出现次数 加入到答案中
        vector<vector<int>> alpha(A.size(), vector<int>(26, 0));
        for (int i = 0; i < A.size(); i++) {
            for (const auto c: A[i]) {
                alpha[i][c-'a']++;
            }
        }
        vector<string> ans;
        for (int i = 0; i < 26; ++i) {
            int count = INT_MAX;
            for (int j = 0; j < A.size(); ++j) {
                count = min(count, alpha[j][i]);
                if (!count) break;
            }
            for (int k = 0; k < count; ++k)
                ans.push_back(string(1, 'a'+i));
        }
        return ans;
    }
};