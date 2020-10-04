// 给你一个由大小写英文字母组成的字符串 s 。
// 一个整理好的字符串中，两个相邻字符 s[i] 和 s[i + 1] 不会同时满足下述条件：
//     0 <= i <= s.length - 2
//     s[i] 是小写字符，但 s[i + 1] 是相同的大写字符；反之亦然 。
// 请你将字符串整理好，每次你都可以从字符串中选出满足上述条件的 两个相邻 字符并删除，直到字符串整理好为止。
// 请返回整理好的 字符串 。题目保证在给出的约束条件下，测试样例对应的答案是唯一的。
// 注意：空字符串也属于整理好的字符串，尽管其中没有任何字符。

// 输入：s = "leEeetcode"
// 输出："leetcode"
// 解释：无论你第一次选的是 i = 1 还是 i = 2，都会使 "leEeetcode" 缩减为 "leetcode" 。

// 输入：s = "abBAcC"
// 输出：""
// 解释：存在多种不同情况，但所有的情况都会导致相同的结果。例如：
// "abBAcC" --> "aAcC" --> "cC" --> ""
// "abBAcC" --> "abBA" --> "aA" --> ""

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/make-the-string-great
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 简单来说; 如果连续两个字符大小写不同 但字母相同; 那就干掉这两个字母
class Solution {
public:
    string makeGood(string s) {
        list<char> stk;
        for (auto it = s.begin(); it != s.end(); ++it) {
            // 1. 栈为空直接入栈
            // 2. 栈不为空; 如果当前元素和栈顶元素大小写不同
            // 2.1  且当前元素和栈顶元素的 大写or小写相同; 则栈顶弹出
            if (!stk.empty() && 
                ((isupper(*it) && islower(stk.back())) || (islower(*it) && isupper(stk.back()))) &&
                toupper(stk.back()) == toupper(*it))
                stk.pop_back();
            else
                stk.push_back(*it);
        }
        return string(stk.begin(), stk.end());
    }
};