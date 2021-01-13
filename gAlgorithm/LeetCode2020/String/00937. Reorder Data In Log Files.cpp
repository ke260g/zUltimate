/*
 * @lc app=leetcode id=937 lang=cpp
 *
 * [937] Reorder Data in Log Files
 *
 * https://leetcode.com/problems/reorder-data-in-log-files/description/
 *
 * algorithms
 * Easy (54.28%)
 * Total Accepted:    183K
 * Total Submissions: 336.9K
 * Testcase Example:  '["dig1 8 1 5 1","let1 art can","dig2 3 6","let2 own kit dig","let3 art zero"]'
 *
 * You have an array of logs.  Each log is a space delimited string of words.
 * 
 * For each log, the first word in each log is an alphanumeric identifier.
 * Then, either:
 * 
 * 
 * Each word after the identifier will consist only of lowercase letters,
 * or;
 * Each word after the identifier will consist only of digits.
 * 
 * 
 * We will call these two varieties of logs letter-logs and digit-logs.  It is
 * guaranteed that each log has at least one word after its identifier.
 * 
 * Reorder the logs so that all of the letter-logs come before any digit-log.
 * The letter-logs are ordered lexicographically ignoring identifier, with the
 * identifier used in case of ties.  The digit-logs should be put in their
 * original order.
 * 
 * Return the final order of the logs.
 * 
 * 
 * Example 1:
 * Input: logs = ["dig1 8 1 5 1","let1 art can","dig2 3 6","let2 own kit
 * dig","let3 art zero"]
 * Output: ["let1 art can","let3 art zero","let2 own kit dig","dig1 8 1 5
 * 1","dig2 3 6"]
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= logs.length <= 100
 * 3 <= logs[i].length <= 100
 * logs[i] is guaranteed to have an identifier, and a word after the
 * identifier.
 * 
 * 
 */
class Solution {
public:
    bool isletter(const string & s) {
        for (int i = 0; i < s.size(); i++)
            if (s[i] == ' ')
                return isalpha(s[i+1]);
        return false;
    }

    static bool cmpLetter(const string &x, const string &y) {
        int l = 0, r = 0;
        while (x[l] != ' ') l++;
        while (y[r] != ' ') r++;
        // 0. 先找到 identifier 和 content 的分割 
        // 1. 比较 content ; 如果 content 不同; 返回 content 的比较结果
        // 2. 比较 identifier 的前面若干个字符(较短的 identifier的长度)
        //    如果能前若干个字符不同; 返回比较结果
        // 3. identifier 前若干个字符相同; 
        //    那就比较identifier长度
        int content= strcmp(x.c_str()+l+1, y.c_str()+r+1);
        if (content != 0)
            return content < 0 /* 1. */;
        int domain = strncmp(x.c_str(), y.c_str(), min(l, r));
        return domain != 0 ? domain < 0 /* 2. */ : l < r /* 3. */;
    }

    vector<string> reorderLogFiles(vector<string>& logs) {
        // 判断 letter-log or digit-log 的方式: 第一个0的右边是 数字 还是 字母

        // 经典双指针 窗口法
        // [0, digit) letter-log
        // [digit, i) digit-log
        // [i, end)   unknown
        int digit = 0;
        for (int i = 0; i < logs.size(); i++) {
            if (isletter(logs[i])) {
                for (int j = i; j > digit; j--)
                    swap(logs[j-1], logs[j]); 
                digit++;
            }
        }
        sort(logs.begin(), logs.begin()+digit, cmpLetter);
        return logs;
    }
};
