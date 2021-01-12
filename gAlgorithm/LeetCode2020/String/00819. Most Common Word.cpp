/*
 * @lc app=leetcode id=819 lang=cpp
 *
 * [819] Most Common Word
 *
 * https://leetcode.com/problems/most-common-word/description/
 *
 * algorithms
 * Easy (45.37%)
 * Total Accepted:    205.9K
 * Total Submissions: 453.4K
 * Testcase Example:  '"Bob hit a ball, the hit BALL flew far after it was hit."\n["hit"]'
 *
 * Given a paragraph and a list of banned words, return the most frequent word
 * that is not in the list of banned words.  It is guaranteed there is at least
 * one word that isn't banned, and that the answer is unique.
 * 
 * Words in the list of banned words are given in lowercase, and free of
 * punctuation.  Words in the paragraph are not case sensitive.  The answer is
 * in lowercase.
 * 
 * 
 * 
 * Example:
 * 
 * 
 * Input: 
 * paragraph = "Bob hit a ball, the hit BALL flew far after it was hit."
 * banned = ["hit"]
 * Output: "ball"
 * Explanation: 
 * "hit" occurs 3 times, but it is a banned word.
 * "ball" occurs twice (and no other word does), so it is the most frequent
 * non-banned word in the paragraph. 
 * Note that words in the paragraph are not case sensitive,
 * that punctuation is ignored (even if adjacent to words, such as "ball,"), 
 * and that "hit" isn't the answer even though it occurs more because it is
 * banned.
 * 
 * 
 * 
 * 
 * Note: 
 * 
 * 
 * 1 <= paragraph.length <= 1000.
 * 0 <= banned.length <= 100.
 * 1 <= banned[i].length <= 10.
 * The answer is unique, and written in lowercase (even if its occurrences in
 * paragraph may have uppercase symbols, and even if it is a proper noun.)
 * paragraph only consists of letters, spaces, or the punctuation symbols
 * !?',;.
 * There are no hyphens or hyphenated words.
 * Words only consist of letters, never apostrophes or other punctuation
 * symbols.
 * 
 * 
 */
#include <vector>
#include <string>
#include <cctype>
using namespace std;
class Solution {
    public:
#if 0 // 方法2: 题解, 较慢; 3次遍历
        string mostCommonWord(string paragraph, vector<string>& banned) {
            // 1. 先完全遍历 一次 paragraph; 转化为小写
            // 2. 再遍历一次; 转化为 stringstream
            // 3. 然后遍历 stringstream 提取每个单词
            // 4. 单词不在 banned 中 且 频率较大 更新
            for (auto & c: paragraph)
                c = ispunct(c) ? ' ' : tolower(c); 
            string w, target;
            map<string, int> freq;
            unordered_set<string> mban(banned.begin(), banned.end());
            stringstream ss(paragraph);
            while (ss >> w) {
                cout << w << endl;
                if (!mban.count(w) &&  ++freq[w] > freq[target])
                    target = w;
            }
            return target;
        }
#endif

#if 1 // 方法1: 自己想的; 较快; 流程复杂
        string mostCommonWord(string paragraph, vector<string>& banned) {
            // 1. banned list; 构造为 set
            // 2. 遍历 paragraph;
            // 3. 非字母; 重置 单词开头
            // 4. 如果是字母那就转化为小写; 
            // 5. 记录 单词开头
            // 6. 如果是单词结尾; 
            //    取词; string(单词开头, i+1)
            //    判断频率是否大于 hits && 不在 banned中; 如果是; 那就更新

            string target;
            map<string, int> freq; 
            set<string> mban(banned.begin(), banned.end()); // 1.
            int head = -1;
            for (int i = 0; i < paragraph.size(); ++i) {    // 2.
                if (!isalpha(paragraph[i])) {               // 3.
                    head = -1;
                    continue;
                }

                // 4.
                paragraph[i] = tolower(paragraph[i]);

                // 5.
                if (head == -1) head = i;  // 单词开头

                // 6.
                if (i+1 == paragraph.size() || !isalpha(paragraph[i+1])) {
                    string w(paragraph.begin()+head, paragraph.begin()+i+1);
                    if (mban.count(w) == 0 && ++freq[w] > freq[target])
                        target = w; 
                }
            }
            return target;
        }
#endif
};
