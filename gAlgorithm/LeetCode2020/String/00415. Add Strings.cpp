// Given two non-negative integers num1 and num2 represented as string, 
// return the sum of num1 and num2.

// Note:

//     The length of both num1 and num2 is < 5100.
//     Both num1 and num2 contains only digits 0-9.
//     Both num1 and num2 does not contain any leading zero.
//     You must not use any built-in BigInteger library or convert the inputs to integer directly.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/add-strings
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// return to_string(stoull(num1) + stoull(num2)) 这个是不行的; 精度不够

// 自己想的; 100% 77%
// 形同 00067 Add Binary
class Solution {
public:
    vector<int> __addVectors(vector<int> &x, vector<int> &y) {
        int i = 0, give = 0;
        auto addone = [&](int &l, int r, int &give) {
            l += r + give; 
            give = l / 10;    
            l %= 10;
        };
        if (x.size() < y.size())
            swap(x, y);
        for (; i < y.size(); ++i)
            addone(x[i], y[i], give);
        for (; i < x.size() && give; ++i)
            addone(x[i], 0, give);
        if (give) 
            x.push_back(give);
        return x;
    }
    string addStrings(string num1, string num2) {
        vector<int> x(num1.begin(), num1.end());
        vector<int> y(num2.begin(), num2.end());
        for (auto & n: x) n -= '0';
        for (auto & n: y) n -= '0';
        reverse(x.begin(), x.end());
        reverse(y.begin(), y.end());

        vector<int> z = __addVectors(x, y);
        for (auto & n: z) n += '0';
        return string(z.rbegin(), z.rend());
    }
};
