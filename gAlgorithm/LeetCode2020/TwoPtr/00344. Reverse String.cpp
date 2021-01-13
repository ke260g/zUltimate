// Input: ["h","e","l","l","o"]
// Output: ["o","l","l","e","h"]

// Input: ["H","a","n","n","a","h"]
// Output: ["h","a","n","n","a","H"]

// 翻转字符串

#if 0
class Solution { // 索引标记法
public:
    void reverseString(vector<char>& s) {
        if (s.empty()) return;
        int mid = s.size() % 2 ? (s.size()-1) / 2 : (s.size()/2);
        for (int i = 0; i < mid; ++i) {
            swap(s[i], s[s.size()-i-1]);
        }
    }
};
#endif

class Solution { // 双指针前后逼近法
public:
    void reverseString(vector<char>& s) {
        if (s.empty()) return;
        int head = 0, tail = s.size()-1;
        while (head < tail)
            swap(s[head++], s[tail--]);
    }
};
