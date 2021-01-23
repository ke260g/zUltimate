/*
 * @lc app=leetcode id=1370 lang=cpp
 *
 * [1370] Increasing Decreasing String
 *
 * https://leetcode.com/problems/increasing-decreasing-string/description/
 *
 * algorithms
 * Easy (76.28%)
 * Total Accepted:    31.9K
 * Total Submissions: 41.8K
 * Testcase Example:  '"aaaabbbbcccc"'
 *
 * Given a string s. You should re-order the string using the following
 * algorithm:
 * 
 * 
 * Pick the smallest character from s and append it to the result.
 * Pick the smallest character from s which is greater than the last appended
 * character to the result and append it.
 * Repeat step 2 until you cannot pick more characters.
 * Pick the largest character from s and append it to the result.
 * Pick the largest character from s which is smaller than the last appended
 * character to the result and append it.
 * Repeat step 5 until you cannot pick more characters.
 * Repeat the steps from 1 to 6 until you pick all characters from s.
 * 
 * 
 * In each step, If the smallest or the largest character appears more than
 * once you can choose any occurrence and append it to the result.
 * 
 * Return the result string after sorting s with this algorithm.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: s = "aaaabbbbcccc"
 * Output: "abccbaabccba"
 * Explanation: After steps 1, 2 and 3 of the first iteration, result = "abc"
 * After steps 4, 5 and 6 of the first iteration, result = "abccba"
 * First iteration is done. Now s = "aabbcc" and we go back to step 1
 * After steps 1, 2 and 3 of the second iteration, result = "abccbaabc"
 * After steps 4, 5 and 6 of the second iteration, result = "abccbaabccba"
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: s = "rat"
 * Output: "art"
 * Explanation: The word "rat" becomes "art" after re-ordering it with the
 * mentioned algorithm.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: s = "leetcode"
 * Output: "cdelotee"
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: s = "ggggggg"
 * Output: "ggggggg"
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: s = "spo"
 * Output: "ops"
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= s.length <= 500
 * s contains only lower-case English letters.
 * 
 * 
 */
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
// #include "prettyprint.hpp"
#include <cstdio>
using namespace std;

class Solution {
public:
    string sortString(string s) {
        // 1. 排序
        // 2. 构造 list[] = { alpha, count }
        // 3. 变换方向地 遍历 list[];
        string answer(s.size(), '\0');
        vector<pair<char, int>> visit;
        sort(s.begin(), s.end());

        for (int i = 0, count = 0; i < s.size(); ++i) {
            count++;
            if (i+1 == s.size() || s[i] != s[i+1]) {
                visit.push_back(make_pair(s[i], count));
                count = 0;
            }
        }
        int k = 0;
        while (k < answer.size()) {
            int i = 0;
            while (k < answer.size() && i < visit.size()) {
                if (visit[i].second > 0) {
                    answer[k++] = visit[i].first;
                    visit[i].second--;
                }
                i++;
            }
            i = visit.size()-1;

            while (k < answer.size() && i >= 0) {
                if (visit[i].second > 0) {
                    answer[k++] = visit[i].first;
                    visit[i].second--;
                }
                i--;
            }
        }
        return answer;
    }
};
