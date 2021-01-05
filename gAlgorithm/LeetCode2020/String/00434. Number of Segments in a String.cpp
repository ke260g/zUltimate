// You are given a string s, return the number of segments in the string. 
// A segment is defined to be a contiguous sequence of non-space characters.

// Input: s = "Hello, my name is John"
// Output: 5
// Explanation: The five segments are ["Hello,", "my", "name", "is", "John"]

// Input: s = "Hello"
// Output: 1

// Input: s = "love live! mu'sic forever"
// Output: 4

// Input: s = ""
// Output: 0

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/number-of-segments-in-a-string
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 求出被空格分割的 非空格字符段个数
// 遍历字符串
// 当 当前字符不位空格 且 上一个字符是空格时; 计数++
// 
// 我们假设字符串的前面 加上了一个 空格
// 所以初始时; 上一个字符我可以假定为空格
// 自己想的; 100%, 60%
class Solution {
public:
    int countSegments(string s) {
        char prev = ' ';
        int count = 0;
        for (auto c: s) {
            if (prev == ' ' && c != ' ')
                count++;
            prev = c;
        }
        return count;
    }
};