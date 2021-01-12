/*
 * @lc app=leetcode id=859 lang=cpp
 *
 * [859] Buddy Strings
 *
 * https://leetcode.com/problems/buddy-strings/description/
 *
 * algorithms
 * Easy (29.71%)
 * Total Accepted:    80.7K
 * Total Submissions: 271.7K
 * Testcase Example:  '"ab"\n"ba"'
 *
 * Given two strings A and B of lowercase letters, return true if you can swap
 * two letters in A so the result is equal to B, otherwise, return false.
 * 
 * Swapping letters is defined as taking two indices i and j (0-indexed) such
 * that i != j and swapping the characters at A[i] and A[j]. For example,
 * swapping at indices 0 and 2 in "abcd" results in "cbad".
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: A = "ab", B = "ba"
 * Output: true
 * Explanation: You can swap A[0] = 'a' and A[1] = 'b' to get "ba", which is
 * equal to B.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: A = "ab", B = "ab"
 * Output: false
 * Explanation: The only letters you can swap are A[0] = 'a' and A[1] = 'b',
 * which results in "ba" != B.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: A = "aa", B = "aa"
 * Output: true
 * Explanation: You can swap A[0] = 'a' and A[1] = 'a' to get "aa", which is
 * equal to B.
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: A = "aaaaaaabc", B = "aaaaaaacb"
 * Output: true
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: A = "", B = "aa"
 * Output: false
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= A.length <= 20000
 * 0 <= B.length <= 20000
 * A and B consist of lowercase letters.
 * 
 * 
 */
class Solution {
public:
    bool buddyStrings(string A, string B) {
        // 1. 长度不一; 不满足条件
        // 2. 从左往右遍历; 找到第一个不同点; (顺便记录 字母出现次数)
        // 3. 找不到不同点= 两字符串完全相同;
        //    如果存在出现两次or以上的字母; 则满足条件
        //    否则不满足 (因为题意 必须交换一次)
        // 4. 从右往左遍历; 找到第二个不同点; (不再记录 字母出现次数)
        // 5. 找不到第二个不同点; 说明有且只有一个字母不同;
        //    不满足条件
        // 6. 当找到两个不同点; swap 其中一个字符串的 两个不同点的位置

        // 1. 长度不一
        if (A.size() != B.size()) return false;
        int alpha[26] = { 0 };
        int head = -1, tail = -1;

        // 2. 从左往右遍历; 找到第一个不同点
        for (int i = 0; i < A.size(); i++) {
            ++alpha[A[i]-'a'];
            if (A[i] != B[i]) {
                head = i;
                break;
            }
        }
        // 3. 两字符串完全相同; 判断是否存在 出现两次的字母
        if (head < 0) 
            return any_of(alpha, alpha+26, [](int n) { return n > 1; });
       
        // 4. 从右往左遍历; 找到第二个不同点
        for (int j = A.size()-1; j > head; j--) {
            if (A[j] != B[j]) {
                tail = j;
                break;
            }
        }
        // 5. 两字符串只有一个字符不同; 不满足题意
        if (tail < 0) 
            return false;

        // 6. 交换两个不同点后; 遍历两个不同点间的字符
        //    注意; 包含不同点本身; 因为swap了; 要重新比较
        //    如果找到第三个不同点; 那就不满足题意
        swap(A[head], A[tail]);
        for (int i = head; i <= tail; ++i)
            if (A[i] != B[i])
                return false;

        return true;
    }
};
