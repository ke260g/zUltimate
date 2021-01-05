// Given a string, find the first non-repeating character in it and return its index.
// If it doesn't exist, return -1.

// s = "leetcode"
// return 0.

// s = "loveleetcode"
// return 2.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/first-unique-character-in-a-string
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 题目的意思: 返回第一个 唯一的字符的索引

// 自己想的: 90%, 90%
// 1. 遍历第一次: 记录每个字母出现的次数
// 2. 遍历第二次: 查询当前位置字母的次数
//    如果满足条件; 则返回索引
class Solution {
public:
    int firstUniqChar(string s) {
        static int a[26];
        memset(a, 0, 26*sizeof(int));
        for (int i = 0; i < s.size(); ++i)
            a[s[i]-'a']++;
        for (int i = 0; i < s.size(); ++i)
            if (a[s[i]-'a'] == 1)
                return i;
        return -1;
    }
};

// 函数静态数组(heap 内存)
// 消耗内存相较于栈更少
// 我猜 可能 调用测试集时; 多次调用这个函数; 导致栈内存的叠加?
// 这种叠加是因为多线程??