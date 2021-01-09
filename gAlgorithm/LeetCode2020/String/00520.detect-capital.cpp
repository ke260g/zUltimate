/*
 * @lc app=leetcode id=520 lang=cpp
 *
 * [520] Detect Capital
 *
 * https://leetcode.com/problems/detect-capital/description/
 *
 * algorithms
 * Easy (53.85%)
 * Total Accepted:    181.2K
 * Total Submissions: 336.6K
 * Testcase Example:  '"USA"'
 *
 * Given a word, you need to judge whether the usage of capitals in it is right
 * or not.
 * 
 * We define the usage of capitals in a word to be right when one of the
 * following cases holds:
 * 
 * 
 * All letters in this word are capitals, like "USA".
 * All letters in this word are not capitals, like "leetcode".
 * Only the first letter in this word is capital, like "Google".
 * 
 * Otherwise, we define that this word doesn't use capitals in a right way.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: "USA"
 * Output: True
 * 
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "FlaG"
 * Output: False
 * 
 * 
 * 
 * 
 * Note: The input will be a non-empty word consisting of uppercase and
 * lowercase latin letters.
 * 
 */
class Solution {
    private:
        static bool __islower(int c) { return islower(c); }
        static bool __isupper(int c) { return isupper(c); }
    public:
        bool detectCapitalUse(string word) {
            // 1. 空字符串是 false
            // 2. 单字符肯定 true
            // 3. w[0] w[1] 都是大写, 那么后续必须全是大写
            // 4. w[0] 是大写 w[1] 是小写, 那么后续必须全是小写
            // 5. w[0] 是小写, 那么后续必须全是小写
            //
            // 注意 all_of 回调要求参数是 int;
            // 原生的 lower/upper 只有char
            if (word.empty()) return false;
            if (word.size() < 2) return true;      
            if (isupper(word[0])) {
                if (isupper(word[1])) { // 2~end 全是 upper
                    return all_of(word.begin()+2, word.end(), __isupper);

                } else {                // 2~end 全是 lower
                    return all_of(word.begin()+2, word.end(), __islower);
                }
            }
            // 1~end 全是 lower
            return all_of(word.begin()+1, word.end(), __islower);
        }
};
