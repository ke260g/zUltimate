// 10进制转化为7进制
// 以字符串形式输出
class Solution {
public:
    string convertToBase7(int num) {
        string s;
        bool nega = false;
        if (num == 0)
            return string("0");
        if (num < 0) {
            nega = true;
            num = -num;
        }
        while (num > 0) {
            int res = num % 7;
            s = to_string(res) + s;            
            num = (num - res) / 7;
        }
        if (nega)
            s = "-" + s;
        return s;
    }
};
