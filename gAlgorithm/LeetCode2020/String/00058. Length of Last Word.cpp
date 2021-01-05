// Given a string s consists of some words separated by spaces, 
// return the length of the last word in the string. If the last word does not exist, return 0.

// A word is a maximal substring consisting of non-space characters only.

// Input: s = "Hello World"
// Output: 5

// Input: s = " "
// Output: 0

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/length-of-last-word
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int lengthOfLastWord(string s) {
        int count = 0;
        int i = s.size()-1;
        
        // 先干掉末尾的零
        for (; i >= 0; i--) {
            if (s[i] != ' ')
                break;
        }
        // 再计算某位非零字符的个数
        for (; i >= 0; i--) {
            if (s[i] == ' ')
                break;
            else
                count++;
        }
        return count;
    }
};