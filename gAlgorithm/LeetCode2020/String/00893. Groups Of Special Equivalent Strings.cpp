/*
 * @lc app=leetcode id=893 lang=cpp
 *
 * [893] Groups of Special-Equivalent Strings
 *
 * https://leetcode.com/problems/groups-of-special-equivalent-strings/description/
 *
 * algorithms
 * Easy (68.15%)
 * Total Accepted:    31.5K
 * Total Submissions: 46.2K
 * Testcase Example:  '["abcd","cdab","cbad","xyzz","zzxy","zzyx"]'
 *
 * You are given an array A of strings.
 * 
 * A move onto S consists of swapping any two even indexed characters of S, or
 * any two odd indexed characters of S.
 * 
 * Two strings S and T are special-equivalent if after any number of moves onto
 * S, S == T.
 * 
 * For example, S = "zzxy" and T = "xyzz" are special-equivalent because we may
 * make the moves "zzxy" -> "xzzy" -> "xyzz" that swap S[0] and S[2], then S[1]
 * and S[3].
 * 
 * Now, a group of special-equivalent strings from A is a non-empty subset of A
 * such that:
 * 
 * 
 * Every pair of strings in the group are special equivalent, and;
 * The group is the largest size possible (ie., there isn't a string S not in
 * the group such that S is special equivalent to every string in the group)
 * 
 * 
 * Return the number of groups of special-equivalent strings from A.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: ["abcd","cdab","cbad","xyzz","zzxy","zzyx"]
 * Output: 3
 * Explanation: 
 * One group is ["abcd", "cdab", "cbad"], since they are all pairwise special
 * equivalent, and none of the other strings are all pairwise special
 * equivalent to these.
 * 
 * The other two groups are ["xyzz", "zzxy"] and ["zzyx"].  Note that in
 * particular, "zzxy" is not special equivalent to "zzyx".
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: ["abc","acb","bac","bca","cab","cba"]
 * Output: 3
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * 1 <= A.length <= 1000
 * 1 <= A[i].length <= 20
 * All A[i] have the same length.
 * All A[i] consist of only lowercase letters.
 * 
 * 
 * 
 * 
 * 
 * 
 */

#ifdef DEV
#include "prettyprint.hpp"
#endif

#include <iostream>
#include <vector>
#include <string>
#include <set>

using namespace std;

class Solution {
public:
    int numSpecialEquivGroups(vector<string>& A) {
        // 1. 按照题意; 每个字符串可以解析为一个 52个成员的数组
        //    前26个成员记录 奇数位置 小写字母的出现频率
        //    后26个成员记录 偶数位置 小写字母的出现频率
        //    满足条件的两个字符串其数组是完全相等的
        // 2. 遍历每个字符串; 解析成 数组 (uuid)
        // 3. 找一下之该数组是否存在
        // 4. 如果没有; 那么该数组加入集合set中
        //    符合条件的组合++
        //
        // Note: array 性能高于定长的 vector
        set<array<int, 52>> grps;
        int count = 0; 

        for (const auto & s: A) {
            array<int, 52> grp = { 0 };

            for (int i = 0; i < s.size(); i++)
                grp[s[i]-'a'+(0x1 & i ? 26 : 0)]++; 

            if (!grps.count(grp)) {
                grps.insert(grp);
                count++;
            }
        }
        return count;
    }
};
