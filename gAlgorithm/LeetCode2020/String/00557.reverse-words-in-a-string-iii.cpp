/*
 * @lc app=leetcode id=557 lang=cpp
 *
 * [557] Reverse Words in a String III
 *
 * https://leetcode.com/problems/reverse-words-in-a-string-iii/description/
 *
 * algorithms
 * Easy (71.44%)
 * Total Accepted:    235.5K
 * Total Submissions: 329.6K
 * Testcase Example:  '"Let\'s take LeetCode contest"'
 *
 * Given a string, you need to reverse the order of characters in each word
 * within a sentence while still preserving whitespace and initial word order.
 * 
 * Example 1:
 * 
 * Input: "Let's take LeetCode contest"
 * Output: "s'teL ekat edoCteeL tsetnoc"
 * 
 * 
 * 
 * Note:
 * In the string, each word is separated by single space and there will not be
 * any extra space in the string.
 * 
 */
class Solution {
public:
    void __reverseWords(string &s, int head, int tail) {
        int i = head, j = tail;
        while (i < j)
            swap(s[i++], s[j--]);
    }

    string reverseWords(string s) {
        // 1. 实现指定范围的翻转函数
        // 2. 从空格到非空格 记录索引为单词开头
        // 3. 从非空格到空格 索引-1 为单词尾
        //    拿到 单词头尾的索引 后调用 指定范围的翻转函数
        // 4. 最后; 如果末尾不是空格字符; 说明漏掉了一个单词
        //    再调用一次 指定范围的翻转函数
        char prev = ' ';
        int head = 0;
        for (int i = 0; i < s.size(); ++i) {
            char curr = s[i];
            if (prev == ' ' && curr != ' ') 
                head = i;
            else if (prev != ' ' && curr == ' ')
                __reverseWords(s, head, i-1);
            prev = curr;
        }
        if (s[s.size()-1] != ' ')
            __reverseWords(s, head, s.size()-1);
        return s;
    }
};
