/*
 * @lc app=leetcode id=824 lang=cpp
 *
 * [824] Goat Latin
 *
 * https://leetcode.com/problems/goat-latin/description/
 *
 * algorithms
 * Easy (66.28%)
 * Total Accepted:    107.6K
 * Total Submissions: 162.3K
 * Testcase Example:  '"I speak Goat Latin"'
 *
 * A sentence S is given, composed of words separated by spaces. Each word
 * consists of lowercase and uppercase letters only.
 * 
 * We would like to convert the sentence to "Goat Latin" (a made-up language
 * similar to Pig Latin.)
 * 
 * The rules of Goat Latin are as follows:
 * 
 * 
 * If a word begins with a vowel (a, e, i, o, or u), append "ma" to the end of
 * the word.
 * For example, the word 'apple' becomes 'applema'.
 * 
 * If a word begins with a consonant (i.e. not a vowel), remove the first
 * letter and append it to the end, then add "ma".
 * For example, the word "goat" becomes "oatgma".
 * 
 * Add one letter 'a' to the end of each word per its word index in the
 * sentence, starting with 1.
 * For example, the first word gets "a" added to the end, the second word gets
 * "aa" added to the end and so on.
 * 
 * 
 * Return the final sentence representing the conversion from S to Goat
 * Latin. 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: "I speak Goat Latin"
 * Output: "Imaa peaksmaaa oatGmaaaa atinLmaaaaa"
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "The quick brown fox jumped over the lazy dog"
 * Output: "heTmaa uickqmaaa rownbmaaaa oxfmaaaaa umpedjmaaaaaa overmaaaaaaa
 * hetmaaaaaaaa azylmaaaaaaaaa ogdmaaaaaaaaaa"
 * 
 * 
 * 
 * 
 * Notes:
 * 
 * 
 * S contains only uppercase, lowercase and spaces. Exactly one space between
 * each word.
 * 1 <= S.length <= 150.
 * 
 * 
 */
#include <string>
#include <cctype>
using namespace std;
class Solution {
public:
    string toGoatLatin(string S) {
        int index = 0, start = -1;
        string ans;
        // 1. 遍历字符串
        // 2. 非字母; 直接append; 重置 单词头
        // 3. 是字母; 设置单词头
        // 4. 取词(i 是结尾 or 下一个字符不是字母)
        // 5. 按照提议 append 即可
        for (int i = 0; i < S.size(); ++i) {
            if (!isalpha(S[i])) {
                ans.append(string(S.begin()+i, S.begin()+i+1));
                start = -1;
                continue;
            }

            if (start < 0) start = i;

            if (i+1 == S.size() || !isalpha(S[i+1])) {
                char first = tolower(S[start]);
                if (first == 'a' || first == 'e' || first == 'i' || first == 'o' || first == 'u')
                    ans.append(string(S.begin()+start, S.begin()+i+1));
                else {
                    ans.append(string(S.begin()+start+1, S.begin() +i +1));
                    ans.append(string(S.begin()+start, S.begin()+start+1));
                }
                ans.append("ma");
                ans.append(string(++index, 'a'));
            }
        }
        return ans;
    }
};
