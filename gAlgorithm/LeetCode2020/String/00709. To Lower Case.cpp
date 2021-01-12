/*
 * @lc app=leetcode id=709 lang=cpp
 *
 * [709] To Lower Case
 *
 * https://leetcode.com/problems/to-lower-case/description/
 *
 * algorithms
 * Easy (79.92%)
 * Total Accepted:    242.8K
 * Total Submissions: 303.8K
 * Testcase Example:  '"Hello"'
 *
 * Implement function ToLowerCase() that has a string parameter str, and
 * returns the same string in lowercase.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: "Hello"
 * Output: "hello"
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: "here"
 * Output: "here"
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: "LOVELY"
 * Output: "lovely"
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    string toLowerCase(string str) {
        static const char d = 'a' - 'A';
        for (auto & c : str) {
            if ('A' <= c && c <= 'Z')
                // c = 'a' + c - 'A';
                c += d;
        }
        return str;
    }
};