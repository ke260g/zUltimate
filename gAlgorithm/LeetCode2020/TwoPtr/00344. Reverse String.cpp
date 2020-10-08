// Input: ["h","e","l","l","o"]
// Output: ["o","l","l","e","h"]

// Input: ["H","a","n","n","a","h"]
// Output: ["h","a","n","n","a","H"]

// 翻转字符串
class Solution {
public:
    void reverseString(vector<char>& s) {
        if (s.empty()) return;
        int mid = s.size() % 2 ? (s.size()-1) / 2 : (s.size()/2);
        for (int i = 0; i < mid; ++i) {
            swap(s[i], s[s.size()-i-1]);
        }
    }
};