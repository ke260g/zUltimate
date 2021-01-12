/*
 * @lc app=leetcode id=43 lang=cpp
 *
 * [43] Multiply Strings
 *
 * https://leetcode.com/problems/multiply-strings/description/
 *
 * algorithms
 * Medium (34.68%)
 * Total Accepted:    338.3K
 * Total Submissions: 975K
 * Testcase Example:  '"2"\n"3"'
 *
 * Given two non-negative integers num1 and num2 represented as strings, return
 * the product of num1 and num2, also represented as a string.
 * 
 * Note: You must not use any built-in BigInteger library or convert the inputs
 * to integer directly.
 * 
 * 
 * Example 1:
 * Input: num1 = "2", num2 = "3"
 * Output: "6"
 * Example 2:
 * Input: num1 = "123", num2 = "456"
 * Output: "56088"
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= num1.length, num2.length <= 200
 * num1 and num2 consist of digits only.
 * Both num1 and num2 do not contain any leading zero, except the number 0
 * itself.
 * 
 * 
 */
#include <vector>
#include <iostream>

#ifdef DEBUG
#include "prettyprint.hpp"
#endif

using namespace std;

#if 1 // 自己的方法1
class Solution {
    public:
    int __addNumber(int x, int y, int & give) {
        x = x + y + give;
        give = x / 10;
        return x % 10;
    }
    int __mulNumber(int x, int y, int & give) {
        x = x * y + give;
        give = x / 10;
        return x % 10;
    }

    vector<int> __addString(vector<int> &x, vector<int> &y) {
        vector<int> z;
        int i = 0, j = 0, give = 0;
        while (i < x.size() && j < y.size())
            z.push_back(__addNumber(x[i++], y[j++], give));
        while (i < x.size())
            z.push_back(__addNumber(x[i++], 0, give));
        while (j < y.size())
            z.push_back(__addNumber(0, y[j++], give));
        if (give)
            z.push_back(give);
        return z;
    }

    vector<int> __mulString(vector<int> &x, int y, int exp) {
        if (y == 0 || (!x.empty() && x.back() == 0)) return vector<int>(1, 0);
        vector<int> z(exp, 0); // exp == 0; 构造vector是合法的
        int i = 0, give = 0;
        while (i < x.size())
            z.push_back(__mulNumber(x[i++], y, give));
        if (give)
            z.push_back(give);
        return z;
    }
    string multiply(string num1, string num2) {
        // 1. 实现字符串加法
        // 2. 实现字符串乘法的简化版: num1 * num2 * (10^exp)
        //    num1 可以是任意正整数; 但 nums2 只能是 0~9,
        //    exp 是 10 的倍数
        // 3. 先把字符串逆序转换为 int 的 vector
        //    逆序: 方便从低位开始计算
        //    int : 减少重复转换
        // 4. 遍历 num2 的每个10进制位; 调用 2.; 求得乘法结果
        //    然后 使用 1. 累加
        // 5. 把结果转化为字符; 在逆序输出

        vector<int> x(num1.size(), 0);
        vector<int> y(num2.size(), 0);

        for (int i = 0; i < num1.size(); ++i)
            x[i] = num1[num1.size()-i-1]-'0';
        for (int i = 0; i < num2.size(); ++i)
            y[i] = num2[num2.size()-i-1]-'0';

        vector<int> s(1, 0);


        for (int i = 0; i < y.size(); i++) {
            vector<int> t = __mulString(x, y[i], i);
            s = __addString(s, t);
        }

        // 3. 转换为 char
        for (auto & n: s) n += '0';
        return string(s.rbegin(), s.rend());
    }
};
#endif

#if 0 // 自己的方法2; 贼慢
class Solution {
    public:
    int __add(int x, int y, int & give) {
        int curr = x + y + give;
        give = curr / 10;
        return curr % 10;
    }
    vector<int> add(const vector<int> &x, const vector<int> &y) {
        vector<int> s;
        int i = 0, j = 0, give = 0;
        while (i < x.size() && j < y.size())
            s.push_back(__add(x[i++], y[j++], give));
        while (i < x.size())
            s.push_back(__add(x[i++], 0, give));
        while (j < y.size())
            s.push_back(__add(0, y[j++], give));
        if (give)
            s.push_back(give);
        return s;
    }
    string multiply(string num1, string num2) {
        // 1. 实现字符串加法
        // 2. 实现字符串乘法的简化版: num1 * num2 * (10^exp)
        //    num1 可以是任意正整数; 但 nums2 只能是 0~9,
        //    exp 是 10 的倍数
        // 3. 遍历 num2 的每个10进制位; 调用 2.
        //    然后 使用 1. 累加

        // 全部用 vector 运算; vector 的成员

        // 1. 转换为 int 方便计算
        vector<int> x(num1.size(), 0);
        vector<int> y(num2.size(), 0);

        for (int i = 0; i < num1.size(); ++i)
            x[i] = num1[num1.size()-i-1]-'0';
        for (int i = 0; i < num2.size(); ++i)
            y[i] = num2[num2.size()-i-1]-'0';

        vector<int> s(1, 0);

        // 2. vector 乘法
        for (int i = 0; i < x.size(); ++i) {
            for (int j = 0; j < y.size(); ++j) {
                // 值域是 [0, 9]; 最大值是 81
                int curr = x[i] * y[j];
                if (curr > 0) {
                    vector<int> t(i+j, 0);
                    while (curr > 0) {
                        t.push_back(curr % 10);
                        curr /= 10;
                    }
                    s = add(s, t);
                }
            }
        }

        // 3. 转换为 char
        for (auto & n: s) n += '0';
        return string(s.rbegin(), s.rend());
    }
};
#endif

#if 0 // 自己的方法3: 超慢
class Solution {
    public:
    char __addNumber(char x, char y, int & give) {
        x = x + y + give;
        give = x / 10;
        return x % 10;
    }

    string __addString(const string &x, const string &y) {
        vector<char> z;
        int i = x.size()-1, j = y.size()-1, give = 0;
        while (i >= 0 && j >= 0)
            z.push_back('0' + __addNumber(x[i--]-'0', y[j--]-'0', give));
        while (i >= 0)
            z.push_back('0' + __addNumber(x[i--]-'0', 0, give));
        while (j >= 0)
            z.push_back('0' + __addNumber(0, y[j--]-'0', give));
        if (give)
            z.push_back('0' + give);
        return string(z.rbegin(), z.rend());
    }
    string multiply(string num1, string num2) {
        // 1. 实现字符串加法
        // 2. 64bit 操作系统;
        //    unsigned long long 表示最大的数值 字符串占据 20 字符
        //    2^64 - 1
        // 3. 规律: str(x*y) == str(x) + str(y) + 1
        // 4. 所以我们取 较长的字符串; 每次取18个; 强转为unsigned long long
        //    在较短字符串每次取 1个字符; 然后进行正常的乘法
        //    然后把值累加(字符串加法); 注意 10 的倍数

        string & x = num1;
        string & y = num2;
        if (x.size() < y.size())
            swap(x, y);

        string z = "0";

        const int postStepN = 18;
        for (int i = x.size(); i > 0; i -= postStepN) {
            string X((i-postStepN) > 0 ? x.begin()+i-postStepN: x.begin(), x.begin()+i);
            const unsigned long long bigX = stoull(X);
            for (int j = y.size(); j > 0; j--) {
                unsigned long long bigZ = bigX * (y[j-1]-'0');
                if (bigZ)
                    z = __addString(z, to_string(bigZ) + string((y.size()-j) + (x.size()-i), '0'));
            }
        }
        return z;
    }
};
#endif

#ifdef DEBUG
int main(void) {
    Solution solution;
    cout << solution.multiply("925101087184894", "3896737933784656127") << endl;
    return 0;
}
#endif
