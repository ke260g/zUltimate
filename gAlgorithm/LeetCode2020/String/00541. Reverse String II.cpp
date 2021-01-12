/*
 * @lc app=leetcode id=541 lang=cpp
 *
 * [541] Reverse String II
 *
 * https://leetcode.com/problems/reverse-string-ii/description/
 *
 * algorithms
 * Easy (48.95%)
 * Total Accepted:    102.5K
 * Total Submissions: 209.4K
 * Testcase Example:  '"abcdefg"\n2'
 *
 * 
 * Given a string and an integer k, you need to reverse the first k characters
 * for every 2k characters counting from the start of the string. If there are
 * less than k characters left, reverse all of them. If there are less than 2k
 * but greater than or equal to k characters, then reverse the first k
 * characters and left the other as original.
 * 
 * 
 * Example:
 * 
 * Input: s = "abcdefg", k = 2
 * Output: "bacdfeg"
 * 
 * 
 * 
 * Restrictions: 
 * 
 * ⁠The string consists of lower English letters only.
 * ⁠Length of the given string and k will in the range [1, 10000]
 * 
 */
class Solution {
public:
    void __reverseStr(string & s, int head, int tail) {
        int i = head, j = tail;
        while (i < j)
           swap(s[i++], s[j--]); 
    }
    string reverseStr(string s, int k) {
        // 1. 实现一个翻转指定 范围的 方法
        // 2. 以 2k 为步进; 遍历 字符串
        //    终止遍历的条件是 i+2*k < s.size(); 保证本次访问小于 2*k 的范围
        //    每次遍历翻转 i, i+k-1 的字符
        // 3. 最后(剩下肯定少于 2k 个)
        //    如果剩下的不足k个, 那么就全部翻转
        //    如果剩下的多于k个, 那么只翻转前面k个
        // 注意: min 要求两个参数的类型相同, 这里要做类型转换
        int i = 0;
        for (; i + 2*k < s.size(); i += 2*k)
          __reverseStr(s, i, i+k-1);

        __reverseStr(s, i, min(i+k-1, (int)(s.size()-1)));
        return s;
    }
};
