/*
 * @lc app=leetcode id=929 lang=cpp
 *
 * [929] Unique Email Addresses
 *
 * https://leetcode.com/problems/unique-email-addresses/description/
 *
 * algorithms
 * Easy (67.18%)
 * Total Accepted:    235.3K
 * Total Submissions: 350.3K
 * Testcase Example:  '["test.email+alex@leetcode.com","test.e.mail+bob.cathy@leetcode.com","testemail+david@lee.tcode.com"]'
 *
 * Every email consists of a local name and a domain name, separated by the @
 * sign.
 * 
 * For example, in alice@leetcode.com, alice is the local name, and
 * leetcode.com is the domain name.
 * 
 * Besides lowercase letters, these emails may contain '.'s or '+'s.
 * 
 * If you add periods ('.') between some characters in the local name part of
 * an email address, mail sent there will be forwarded to the same address
 * without dots in the local name.  For example, "alice.z@leetcode.com" and
 * "alicez@leetcode.com" forward to the same email address.  (Note that this
 * rule does not apply for domain names.)
 * 
 * If you add a plus ('+') in the local name, everything after the first plus
 * sign will be ignored. This allows certain emails to be filtered, for example
 * m.y+name@email.com will be forwarded to my@email.com.  (Again, this rule
 * does not apply for domain names.)
 * 
 * It is possible to use both of these rules at the same time.
 * 
 * Given a list of emails, we send one email to each address in the list.  How
 * many different addresses actually receive mails? 
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input:
 * ["test.email+alex@leetcode.com","test.e.mail+bob.cathy@leetcode.com","testemail+david@lee.tcode.com"]
 * Output: 2
 * Explanation: "testemail@leetcode.com" and "testemail@lee.tcode.com" actually
 * receive mails
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * 1 <= emails[i].length <= 100
 * 1 <= emails.length <= 100
 * Each emails[i] contains exactly one '@' character.
 * All local and domain names are non-empty.
 * Local names do not start with a '+' character.
 * 
 * 
 * 
 */
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_set<string> uniqueEmails;
        for (auto temail : emails) {
            // status
            // 0: local name
            // 1: sign name
            // 2: domain name
            //
            // 1. 逐个遍历 email; 逐个遍历email的字符
            // 2. 先处理字符的状态变换 ('+', '@' 触发状态切换)
            //    '@' 本身也要入列
            // 3. 再处理特定状态下某个字符的动作
            //    local name 且不是 dot; 入列
            //    domain name 入列
            // Note: 2, 3 不建议调换
            int status = 0; 
            string memail;
            for (auto c: temail) {
                if (c == '+') {
                    status = 1;
                } else if (c == '@') {
                    status = 2;
                    memail.push_back(c);
                } else {
                    if ((status == 0 && c != '.') || (status == 2))
                        memail.push_back(c);
                }
            }
            uniqueEmails.insert(memail);
        }
        return uniqueEmails.size();
    }
};
