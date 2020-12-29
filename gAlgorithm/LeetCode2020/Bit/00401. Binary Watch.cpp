// A binary watch has 4 LEDs on the top which represent the hours (0-11), 
// and the 6 LEDs on the bottom represent the minutes (0-59).

// Each LED represents a zero or one, with the least significant bit on the right.

// For example, the above binary watch reads "3:25".

// Given a non-negative integer n which represents the number of LEDs that are currently on,
// return all possible times the watch could represent.

// Input: n = 1
// Return: ["1:00", "2:00", "4:00", "8:00", "0:01", "0:02", "0:04", "0:08", "0:16", "0:32"]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/binary-watch
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 二进制手表

// 方法1: 1. 遍历 12小时 60分钟
//        2. 求每一分钟 占用bit 个数; (00191. Number of 1 Bits 方法)
//        3. 如果 该分钟 占用bit 个数相等; 则返回结果
//        4. 注意 0分钟~9分钟 需要补零"0"
class Solution {
public:
    vector<string> readBinaryWatch(int num) {
        vector<string> ans;
        for (int h = 0; h < 12; h++) {
            for (int m = 0; m < 60; m++) {
                if (nbit(h) + nbit(m) == num)
                    ans.push_back(to_string(h)+":"+ (m < 10 ? "0"+to_string(m) : to_string(m)));
            }
        }
        return ans;
    }
    int nbit(int num) {
        int n = 0;
        while (num) {
            num &= (num - 1);
            n++;
        }
        return n;
    }
};

// 方法2: 全排列
class Solution {
public:
    //   hours     minutes
    // [8,4,2,1, 32,16,8,4,2,1]
    // [0,1,2,3,  4, 5,6,7,8,9]
    void __dumpBinaryWatch(vector<bool> &a, vector<string> &ans) {
        int hours = 0, minutes = 0;
        for (int i = 0; i < 4; i++)
            hours += a[i] ? 1 << (3 - i) : 0;
        for (int i = 4; i < 10; ++i)
            minutes += a[i] ? 1 << (9 - i) : 0;
        if (hours < 12 && minutes < 60) {
            string s = to_string(hours);
            s += ":";
            if (minutes < 10)
                s += "0";
            s += to_string(minutes); 
            ans.push_back(s);
        }
    }
    void __readBinaryWatch(vector<bool> &a, vector<string> &ans, int k, int n) {
        if (k == a.size()) {
            if (n == 0)
                return __dumpBinaryWatch(a, ans);
            else
                return;
        }
        __readBinaryWatch(a, ans, k+1, n);
        if (n > 0) {
           a[k] = true;
            __readBinaryWatch(a, ans, k+1, n-1);
            a[k] = false;
        }
    }
    vector<string> readBinaryWatch(int num) {
        vector<bool> a(10, false);
        vector<string> ans;
        __readBinaryWatch(a, ans, 0, num);
        return ans;
    }
};