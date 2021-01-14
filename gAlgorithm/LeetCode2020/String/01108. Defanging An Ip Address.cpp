/*
 * @lc app=leetcode id=1108 lang=cpp
 *
 * [1108] Defanging an IP Address
 *
 * https://leetcode.com/problems/defanging-an-ip-address/description/
 *
 * algorithms
 * Easy (88.45%)
 * Total Accepted:    259.5K
 * Total Submissions: 293.3K
 * Testcase Example:  '"1.1.1.1"'
 *
 * Given a valid (IPv4) IP address, return a defanged version of that IP
 * address.
 * 
 * A defanged IP address replaces every period "." with "[.]".
 * 
 * 
 * Example 1:
 * Input: address = "1.1.1.1"
 * Output: "1[.]1[.]1[.]1"
 * Example 2:
 * Input: address = "255.100.50.0"
 * Output: "255[.]100[.]50[.]0"
 * 
 * 
 * Constraints:
 * 
 * 
 * The given address is a valid IPv4 address.
 * 
 */
class Solution {
public:
    string defangIPaddr(string address) {
        string ans;
        char s[1];
        // 简单的字符串转换
        for (auto c: address) {
            if (c != '.') {
                s[0] = c;
                ans.append(string(s, s+1));
            } else
                ans.append("[.]");
        }
        return ans;
    }
};