// Given a string s containing just the characters '(', ')', '{', '}', '[' and ']',
//  determine if the input string is valid.

// An input string is valid if:

//     Open brackets must be closed by the same type of brackets.
//     Open brackets must be closed in the correct order.

class Solution {
public:
    bool isValid(string s) {
        stack<char> stk;
        for (int i = 0; i < s.size(); ++i) {
            char need_pair = ' ';
            switch (s[i]) {
                case '(':
                case '[':
                case '{':
                    stk.push(s[i]);
                    break;
                case ')':
                    need_pair = '(';
                    break;
                case ']':
                    need_pair = '[';
                    break;
                case '}':
                    need_pair = '{';
                    break;
                default:
                    break;
            }
            if (need_pair != ' ') {
                if (stk.empty() || need_pair != stk.top())
                    return false;
                else
                    stk.pop();
            }
        }
        return stk.empty() ? true : false;
    }
};