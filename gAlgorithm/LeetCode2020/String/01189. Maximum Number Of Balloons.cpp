/*
 * @lc app=leetcode id=1189 lang=cpp
 *
 * [1189] Maximum Number of Balloons
 *
 * https://leetcode.com/problems/maximum-number-of-balloons/description/
 *
 * algorithms
 * Easy (61.76%)
 * Total Accepted:    52K
 * Total Submissions: 84.2K
 * Testcase Example:  '"nlaebolko"'
 *
 * Given a string text, you want to use the characters of text to form as many
 * instances of the word "balloon" as possible.
 * 
 * You can use each character in text at most once. Return the maximum number
 * of instances that can be formed.
 * 
 * 
 * Example 1:
 * 
 * 
 * 
 * 
 * Input: text = "nlaebolko"
 * Output: 1
 * 
 * 
 * Example 2:
 * 
 * 
 * 
 * 
 * Input: text = "loonbalxballpoon"
 * Output: 2
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: text = "leetcode"
 * Output: 0
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= text.length <= 10^4
 * text consists of lower case English letters only.
 * 
 */
class Solution {
public:
    int maxNumberOfBalloons(string text) {
        // 1. 记录目标数组 每个字母需要的次数
        // 2. 记录给定text 每个字母出现的次数
        // 3. 遍历 目标数组的每个字母
        //    求得 在text 中出现次数 / 需要次数 (即倍数)
        // 4. 求上步骤的最小值
        static const string target("balloon");
        static const struct {
            int alpha;
            int count;
        } parts[] = {
            { 'a', 1 },
            { 'b', 1 },
            { 'n', 1 },
            { 'l', 2 },
            { 'o', 2 },
        };

        int alpha[26] = { 0 };
        int count = text.size();

        for (auto c: text)
            alpha[c-'a']++;

        for (auto & p: parts)
            count = min(count, alpha[p.alpha-'a'] / p.count);
        return count;

    }
};
