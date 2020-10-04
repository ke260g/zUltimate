// Given two strings S and T, return if they are equal when both are typed into empty text editors. 
// '#' means a backspace character.

// Note that after backspacing an empty text, the text will continue empty.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/backspace-string-compare
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


// Input: S = "ab#c", T = "ad#c"
// Output: true
// Explanation: Both S and T become "ac".

// Input: S = "ab##", T = "c#d#"
// Output: true
// Explanation: Both S and T become "".

// 使用栈; 把字符串中退格给干掉
class Solution {
    string doBackspace(string s, char backspace = '#') {
        vector<int> t;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == backspace) {
                if (!t.empty())
                    t.pop_back();
            } else
                t.push_back(s[i]);
        }
        string n(t.begin(), t.end());
        return n;
    }
public:
    bool backspaceCompare(string S, string T) {
        string s = doBackspace(S);
        string t = doBackspace(T);
        return s == t;
    }
};