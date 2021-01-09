// Write a function to find the longest common prefix string amongst an array of strings.

// If there is no common prefix, return an empty string "".

// Input: strs = ["flower","flow","flight"]
// Output: "fl"

// Input: strs = ["dog","racecar","car"]
// Output: ""
// Explanation: There is no common prefix among the input strings.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/longest-common-prefix
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


// 1. 遍历第一个字符串
// 2. 取出第一个字符串中的字符
// 3. 然后遍历剩余字符串中相同 index 的字符
//    一旦发现有字符串长度不够 or 相同index字符不同 则 break
//    检查相同index相同字符的字符串个数 是否 == strs.size()
//    如果是; 说明该字符满足条件; 加入到答案中
//    否则; break
class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        if (strs.empty()) return "";
        int i = 0;
        for (; i < strs[0].size(); ++i) {
            char common = strs[0][i];
            int j = 1;
            for (; j < strs.size(); ++j) {
                if (i >= strs[j].size() ||  strs[j][i] != common)
                    break;
            }
            if (j != strs.size())
                break;
        }
        return string(strs[0].begin(), strs[0].begin()+i);
    }
};
