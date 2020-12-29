// Given a non-negative integer num, 
// return the number of steps to reduce it to zero. 
// If the current number is even, 
// you have to divide it by 2, 
// otherwise, you have to subtract 1 from it.

// Input: num = 14
// Output: 6
// Explanation: 
// Step 1) 14 is even; divide by 2 and obtain 7. 
// Step 2) 7 is odd; subtract 1 and obtain 6.
// Step 3) 6 is even; divide by 2 and obtain 3. 
// Step 4) 3 is odd; subtract 1 and obtain 2. 
// Step 5) 2 is even; divide by 2 and obtain 1. 
// Step 6) 1 is odd; subtract 1 and obtain 0.

// Input: num = 8
// Output: 4
// Explanation: 
// Step 1) 8 is even; divide by 2 and obtain 4. 
// Step 2) 4 is even; divide by 2 and obtain 2. 
// Step 3) 2 is even; divide by 2 and obtain 1. 
// Step 4) 1 is odd; subtract 1 and obtain 0.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/number-of-steps-to-reduce-a-number-to-zero
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 场景模拟  100%, 7.18%
// while num
//   奇数: num 减去1
//   偶数: num 右移1
class Solution {
public:
    int numberOfSteps (int num) {
        int nstep = 0;
        while (num) {
            if (num & 0x1)
                num--;
            else
                num >>= 1;
            nstep++;
        }
        return nstep;
    }
};

// 方法2: 数学归纳
// 步数 == 1的bit位数*2 + 有效0的bit位数 - 1
// 其实跟方法1一样; 复杂度都是 n 的有效bit数 
class Solution {
public:
    int numberOfSteps (int num) {
	int n_set = 0, n_clr = 0; 
        while (num) {
            if (num & 0x1)
                n_set++; 
            else
                n_clr++;
            num >>= 1;
        }
        return n_set << 2 + n_clr;
    }
};























