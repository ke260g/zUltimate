// Given two binary strings a and b, return their sum as a binary string.

// Input: a = "11", b = "1"
// Output: "100"

// Input: a = "1010", b = "1011"
// Output: "10101"

// 自己想的; 100%, 65%
// 二进制加法
// 对比 00415 Add Strings
class Solution {
public:
    int addBinarySingle(int x, int y, int & give) {
        int curr = give + x + y;
        if (curr < 2) {
            give = 0;
            return curr;
        }
        give = 1;
        return curr - 2;
    }
    string addBinary(string a, string b) {
        if (a.empty()) return b;
        if (b.empty()) return a;
        vector<char> s;
        int i = a.size()-1, j = b.size()-1;
        int give = 0;
        while (i >= 0 && j >= 0)
            s.push_back('0'+addBinarySingle(a[i--]-'0',  b[j--]-'0', give));
        while (i >= 0)
            s.push_back('0'+addBinarySingle(a[i--]-'0', 0, give));
        while (j >= 0)
            s.push_back('0'+addBinarySingle(0, b[j--]-'0', give));
        if (give)
            s.push_back('0'+give);
        return string(s.rbegin(), s.rend());
    }
};