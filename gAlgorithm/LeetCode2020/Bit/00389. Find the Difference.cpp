// You are given two strings s and t.

// String t is generated by random shuffling string s and 
// then add one more letter at a random position.

// Return the letter that was added to t.

// Input: s = "abcd", t = "abcde"
// Output: "e"
// Explanation: 'e' is the letter that was added.

// Input: s = "", t = "y"
// Output: "y"

// Input: s = "ae", t = "aea"
// Output: "a"

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/find-the-difference
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// xor 操作即可
class Solution {
public:
    char findTheDifference(string s, string t) {
        char c = t[t.size()-1];
        for (int i = 0; i < s.size(); ++i)
            c ^= s[i] ^ t[i];
        return c;
    }
};