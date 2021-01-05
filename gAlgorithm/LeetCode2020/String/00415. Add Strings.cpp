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
    int addDigit(int x, int y, int & give) {
        int curr = x + y + give;
        if (curr >= 10) {
            give = 1;
            return curr - 10;
        }
        give = 0;
        return curr;
    }
    string addStrings(string num1, string num2) {
        vector<char> s;
        int i = num1.size()-1, j = num2.size()-1;
        int give = 0;
        while (i >= 0 && j >= 0)
            s.push_back('0' + addDigit(num1[i--]-'0', num2[j--]-'0', give));
        while (i >= 0)
            s.push_back('0' + addDigit(num1[i--]-'0', 0, give));
        while (j >= 0)
            s.push_back('0' + addDigit(0, num2[j--]-'0', give));
        if (give)
            s.push_back('0' + give);
        return string(s.rbegin(), s.rend());
    }
};