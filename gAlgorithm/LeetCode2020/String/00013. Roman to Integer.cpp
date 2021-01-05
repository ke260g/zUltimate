// Roman numerals are represented by seven different symbols: I, V, X, L, C, D and M.
// Symbol       Value
// I             1
// V             5
// X             10
// L             50
// C             100
// D             500
// M             1000

// For example, 2 is written as II in Roman numeral, 
// just two one's added together. 12 is written as XII, 
// which is simply X + II. The number 27 is written as XXVII, 
// which is XX + V + II.

// Roman numerals are usually written largest to smallest from left to right.
// However, the numeral for four is not IIII. 
// Instead, the number four is written as IV. 
// Because the one is before the five we subtract it making four. 
// The same principle applies to the number nine, which is written as IX. There are six instances where subtraction is used:

// I can be placed before V (5) and X (10) to make 4 and 9. 
// X can be placed before L (50) and C (100) to make 40 and 90. 
// C can be placed before D (500) and M (1000) to make 400 and 900.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/roman-to-integer
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 自己想的
// 1. 记录上一个字符
// 2. 每个当前字符根据上一个字符确定要增加的数值
// 3. 如果上一个字符和当前字符 组合成 4/9, 40/90, 400/900, 
//    那就增加后再减去上一个字符表示的数值的两倍(之前已经加了一倍了)
class Solution {
public:
    int romanToInt(string s) {
        char prev = ' ';
        int ans = 0;
        for (char c: s) {
            switch (c) {
                case 'I':
                    ans += 1;
                break;
                case 'V':
                    ans += prev == 'I' ? 3 : 5; 
                break;
                case 'X':
                    ans += prev == 'I' ? 8 : 10;
                break;
                case 'L':
                    ans += prev == 'X' ? 30 : 50;
                break;
                case 'C':
                    ans += prev == 'X' ? 80 : 100;
                break;
                case 'D':
                    ans += prev == 'C' ? 300 : 500;
                break;
                case 'M':
                    ans += prev == 'C' ? 800 : 1000;
                break;
            }
            prev = c;
        }
        return ans;
    }
};


// 方法2:
// 1. 从右往左
// 2. 如果当前数值 小于 之前的数值
//    则减去当前数值 (应对 4/9, 40/90, 400/900) 的场景
//    否则 加上当前数值
// 3. 记录当前数值
class Solution {
public:
    int romanToInt(char c) {
        switch (c) {
            case 'I':
                return 1;
            case 'V':
                return 5;
            case 'X':
                return 10;
            case 'L':
                return 50;
            case 'C':
                return 100;
            case 'D':
                return 500;
            case 'M':
                return 1000;
            default:
                return 0;
        }
        return 0;
    }
    int romanToInt(string s) {
        int prev = 0;
        int ans = 0;
        for (int i = s.size()-1; i >= 0; i--) {
            int curr = romanToInt(s[i]);
            ans = curr < prev ? ans - curr : ans + curr;
            prev = curr;
        }
        return ans;
    }
};