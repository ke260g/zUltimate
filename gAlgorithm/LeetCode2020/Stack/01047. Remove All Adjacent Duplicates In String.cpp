// 中规中矩的 单栈操作
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