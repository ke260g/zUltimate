// 转换有符整数为 16 进制字符串
// 实际上就是把ta的 二进制 存储; 用字符串表示
//
// 用 vector<int> 从低位到高位 push_back
// 最后 strings(ans.rbegin(), ans.rend())
// 这里有个坑: 有符号负数 右移; 其最左边的位用了1补充; 而不是0
//             所以还得转换位无符号
// 还有一个坑: 0 没有参与我们的 while 遍历; 要提前返回

class Solution {
public:
    char toHexChar(int n) {
        return n > 9 ? (n-10) + 'a' : n + '0';
    }
    string toHex(int num) {
        if (num == 0) return "0";
        unsigned n = num;
        vector<char> ans;
        while (n) {
            ans.push_back(toHexChar(n& 0xf));
            n >>= 4;
        }
        return string(ans.rbegin(), ans.rend());      
    }
};