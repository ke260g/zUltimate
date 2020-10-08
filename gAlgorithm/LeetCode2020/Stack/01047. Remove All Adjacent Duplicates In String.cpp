// 中规中矩的 单栈操作
// 删除重复的字母; 包括字母本身
// 如果是保留首个重复字母则需要稍微改变一下
class Solution {
public:
    string removeDuplicates(string S) {
        list<char> s;
        for (int i = 0; i < S.size(); ++i) {
            if (!s.empty() && S[i] == s.back())
                s.pop_back();
            else
                s.push_back(S[i]);
        }
        return string(s.begin(), s.end());
    }
};