/*
 * @lc app=leetcode id=917 lang=cpp
 *
 * [917] Reverse Only Letters
 *
 * https://leetcode.com/problems/reverse-only-letters/description/
 *
 * algorithms
 * Easy (58.53%)
 * Total Accepted:    71.6K
 * Total Submissions: 122.2K
 * Testcase Example:  '"ab-cd"'
 *
 * Given a string S, return the "reversed" string where all characters that are
 * not a letter stay in the same place, and all letters reverse their
 * positions.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: "ab-cd"
 * Output: "dc-ba"
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "a-bC-dEf-ghIj"
 * Output: "j-Ih-gfE-dCba"
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: "Test1ng-Leet=code-Q!"
 * Output: "Qedo1ct-eeLg=ntse-T!"
 * 
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * S.length <= 100
 * 33 <= S[i].ASCIIcode <= 122 
 * S doesn't contain \ or "
 * 
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    string reverseOnlyLetters(string S) {
        int i = 0, j = S.size()-1; 
        while (i < j) {
            while (i < j && !isalpha(S[j]))
                j--;
            while (i < j && !isalpha(S[i]))
                i++;
            if (i < j)
                swap(S[i++], S[j--]);
        }
        return S; 
    }
};
