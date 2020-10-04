// 干掉最外层的"()"
// Input: "(()())(())"
// Output: "()()()"
// Explanation: 
// The input string is "(()())(())", with primitive decomposition "(()())" + "(())".
// After removing outer parentheses of each part, this is "()()" + "()" = "()()()".

// Input: "(()())(())(()(()))"
// Output: "()()()()(())"
// Explanation: 
// The input string is "(()())(())(()(()))", with primitive decomposition "(()())" + "(())" + "(()(()))".
// After removing outer parentheses of each part, this is "()()" + "()" + "()(())" = "()()()()(())".

// Input: "()()"
// Output: ""
// Explanation: 
// The input string is "()()", with primitive decomposition "()" + "()".
// After removing outer parentheses of each part, this is "" + "" = "".

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/remove-outermost-parentheses
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    string removeOuterParentheses(string S) {
        // 用两个栈;
        // 1. 一个栈用来缓存最外层的"(", 该栈非空表示当前字符属于 内层")"
        // 2. 一个栈用来缓存  内层的"(", 该栈非空表示当前字符属于 外层
        // 3. 把内层字符缓存到 vector中; 用来返回最终的数组
        stack<char> outer, inner;
        vector<char> s;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == '(') {
                // outer 为空; 优先入栈 outer
                if (outer.empty()) {
                    outer.push(S[i]);
                } else {
                // outer 非空; 当前不属于最外层
                    inner.push(S[i]);
                    s.push_back(S[i]);
                }
            } else {
                // inner 为空; 匹配到最外层的')'
                if (inner.empty()) {
                    if (!outer.empty())
                        outer.pop();
                    else // error
                        return "";       
                } else {
                // inner 非空;
                    s.push_back(S[i]);
                    inner.pop();
                }
            }
        }
        return string(s.begin(), s.end());
    }
};

// 优化版本: 两个栈可以用计数器替代
class Solution {
public:
    string removeOuterParentheses(string S) {
        vector<char> s;
        int outer_count = 0, inner_count = 0;
        for (int i = 0; i < S.size(); ++i) {
            if (S[i] == '(') {
                // outer 为空; 优先入栈 outer
                if (outer_count == 0) {
                    outer_count++;
                } else {
                // outer 非空; 当前不属于最外层
                    inner_count++;
                    s.push_back(S[i]);
                }
            } else {
                // inner 为空; 匹配到最外层的')'
                if (inner_count == 0) {
                    if (outer_count != 0)
                        outer_count--;
                    else // error；字符串有bug
                        return "";       
                } else {
                // inner 非空;
                    inner_count--;
                    s.push_back(S[i]);
                }
            }
        }
        return string(s.begin(), s.end());
    }
};