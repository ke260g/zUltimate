// Given a string, determine if it is a palindrome, considering only alphanumeric characters and ignoring cases.
// Note: For the purpose of this problem, we define empty string as valid palindrome.

// Input: "race a car"
// Output: false

// Input: "race a car"
// Output: false

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/valid-palindrome
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 验证回文字符串 (忽略大小写 和 只关注字母和数字)
// 前后双指针算法 (形同快速排序)
// 1. head++ 和 tail-- 找到符合条件的两个字符
// 2. 进行判断
// 3. 如果符合条件的两个字符 不相等则 return false
//
//
// Note:
//   坑点1: 判定字母和数字: 如果直接用 ascii; 可能忘记具体的数值范围 or 大小
//   坑点2: 忽略大小写

class Solution {
public:
    bool isPalindrome(string s) {
        int head = 0, tail = s.size()-1;
        while (head < tail) {
            while (!isalnum(s[head]) && head < tail)
                head++;
            while (!isalnum(s[tail]) && head < tail)
                tail--;
            if (tolower(s[head++]) != tolower(s[tail--])) 
                return false;
        }
        return true;
    }
};