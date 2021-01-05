// Given a non-empty array of decimal digits representing a non-negative integer,
// increment one to the integer.

// The digits are stored such that the most significant digit is at the head of the list,
// and each element in the array contains a single digit.

// You may assume the integer does not contain any leading zero, except the number 0 itself.

// 1. 从右往左分别++
// 2. 因为 digits[i] 只能是 0~9;
//    所以 digits[i]++ 后, 只能是 1~10,
// 3. 如果 不是10; 那么循环终止; 因为不再有进位
// 4. 如果 是  10; 那么循环继续; digits[i] = 置位0;
// 5. 如果 能跑出循环; 那么一定是 99999 这种形式地数值
//    只需要返回 100000.. 即可
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        for (int i = digits.size()-1; i >= 0; i--) {
            digits[i]++;
            digits[i] %= 10;
            if (digits[i]) return digits;
        }
        vector<int> a(digits.size()+1, 0);
        a[0] = 1;
        return a;
    }
};