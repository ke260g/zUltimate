// Given a string s, find the length of the longest substring without repeating characters.

// Input: s = "abcabcbb"
// Output: 3
// Explanation: The answer is "abc", with the length of 3.

// Input: s = "bbbbb"
// Output: 1
// Explanation: The answer is "b", with the length of 1.

// Input: s = "pwwkew"
// Output: 3
// Explanation: The answer is "wke", with the length of 3.
// Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.

// Input: s = ""
// Output: 0

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/longest-substring-without-repeating-characters
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        map<char, int> cache; // key: char, value: last-indexs
        int result = 0/* 结果 */, length = 0/*临时长度*/, start = 0;
        int i = 0;
        for (int i = 0; i < s.size(); ++i) {
            auto found = cache.find(s[i]);
            // 1. if 遇到一个重复的字符 而且 上一次出现的位置在当前匹配长度后方
            // 2. then 重新开始; 匹配的起始位置=上一次位置+1; 长度 = 当前位置-上一次位置
            //    这种场景下不需要更新结果
            // 3. else 继续往前匹配; 长度++, 更新结果
            if (found != cache.end() && found->second >= start) {
                start = found->second+1;
                length = i - found->second;
            } else {
                length++;
                if (length > result)
                    result = length;
            }
            cache[s[i]] = i;
        }
        return result;
    }
};
