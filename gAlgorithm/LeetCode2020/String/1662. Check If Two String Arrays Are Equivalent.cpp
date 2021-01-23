/*
 * @lc app=leetcode id=1662 lang=cpp
 *
 * [1662] Check If Two String Arrays are Equivalent
 *
 * https://leetcode.com/problems/check-if-two-string-arrays-are-equivalent/description/
 *
 * algorithms
 * Easy (83.87%)
 * Total Accepted:    58.6K
 * Total Submissions: 70.2K
 * Testcase Example:  '["ab", "c"]\n["a", "bc"]'
 *
 * Given two string arrays word1 and word2, return true if the two arrays
 * represent the same string, and false otherwise.
 * 
 * A string is represented by an array if the array elements concatenated in
 * order forms the string.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: word1 = ["ab", "c"], word2 = ["a", "bc"]
 * Output: true
 * Explanation:
 * word1 represents string "ab" + "c" -> "abc"
 * word2 represents string "a" + "bc" -> "abc"
 * The strings are the same, so return true.
 * 
 * Example 2:
 * 
 * 
 * Input: word1 = ["a", "cb"], word2 = ["ab", "c"]
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: word1  = ["abc", "d", "defg"], word2 = ["abcddefg"]
 * Output: true
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= word1.length, word2.length <= 10^3
 * 1 <= word1[i].length, word2[i].length <= 10^3
 * 1 <= sum(word1[i].length), sum(word2[i].length) <= 10^3
 * word1[i] and word2[i] consist of lowercase letters.
 * 
 * 
 */
class Solution {
public:
    bool arrayStringsAreEqual(vector<string>& word1, vector<string>& word2) {
        int i1 = 0, i2 = 0, j1 = 0, j2 = 0;
        while (i1 < word1.size() && i2 < word2.size()) {
            if (word1[i1][j1++] != word2[i2][j2++])
                return false;
            if (j1 == word1[i1].size()) {
                j1 = 0;
                i1++;
            }
            if (j2 == word2[i2].size()) {
                j2 = 0;
                i2++;
            }
        }
        return i1 == word1.size() && i2 == word2.size();
    }
};
