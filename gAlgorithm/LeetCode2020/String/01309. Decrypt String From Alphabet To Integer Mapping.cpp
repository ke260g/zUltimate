/*
 * @lc app=leetcode id=1309 lang=cpp
 *
 * [1309] Decrypt String from Alphabet to Integer Mapping
 *
 * https://leetcode.com/problems/decrypt-string-from-alphabet-to-integer-mapping/description/
 *
 * algorithms
 * Easy (77.25%)
 * Total Accepted:    47.4K
 * Total Submissions: 61.4K
 * Testcase Example:  '"10#11#12"'
 *
 * Given a string s formed by digits ('0' - '9') and '#' . We want to map s to
 * English lowercase characters as follows:
 * 
 * 
 * Characters ('a' to 'i') are represented by ('1' to '9') respectively.
 * Characters ('j' to 'z') are represented by ('10#' to '26#') respectively. 
 * 
 * 
 * Return the string formed after mapping.
 * 
 * It's guaranteed that a unique mapping will always exist.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "10#11#12"
 * Output: "jkab"
 * Explanation: "j" -> "10#" , "k" -> "11#" , "a" -> "1" , "b" -> "2".
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "1326#"
 * Output: "acz"
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "25#"
 * Output: "y"
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: s = "12345678910#11#12#13#14#15#16#17#18#19#20#21#22#23#24#25#26#"
 * Output: "abcdefghijklmnopqrstuvwxyz"
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 1000
 * s[i] only contains digits letters ('0'-'9') and '#' letter.
 * s will be valid string such that mapping is always possible.
 * 
 * 
 */
class Solution {
public:
    string freqAlphabets(string s) {
        // 1. 如果是数字; 那么直接 转义再 append
        // 2. 如果是 '#'; 那么answer 后面两个是非法的
        //    先erase 掉一个最后一个
        //    再把最后一个转义为正确的
        //     ((s[i-2]-'0')*10 + s[i-1]-'0') 求得数值
        // Note: 注意这里是1 开始表示 'a'
        string answer;
        int i = s.size()-1;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '#') {
                answer.erase(answer.end()-1, answer.end());
                *(answer.end()-1) = ('a'-1) + ((s[i-2]-'0')*10 + s[i-1]-'0');
            } else
                answer.append(1, ('a'-1) + (s[i]-'0'));
        }
        return answer;
    }
};
