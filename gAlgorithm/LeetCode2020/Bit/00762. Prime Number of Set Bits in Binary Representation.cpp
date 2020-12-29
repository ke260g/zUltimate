// Given two integers L and R, find the count of numbers in the range [L, R] (inclusive) 
// having a prime number of set bits in their binary representation.

// (Recall that the number of set bits an integer has is the number of 1s present when written in binary. 
// For example, 21 written in binary is 10101 which has 3 set bits. Also, 1 is not a prime.)

// Input: L = 6, R = 10
// Output: 4
// Explanation:
// 6 -> 110 (2 set bits, 2 is prime)
// 7 -> 111 (3 set bits, 3 is prime)
// 9 -> 1001 (2 set bits , 2 is prime)
// 10->1010 (2 set bits , 2 is prime)

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/prime-number-of-set-bits-in-binary-representation
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int countBits(int n) {
        int count = 0;
        while (n) {
            n = (n & (n - 1));
            count++;
        }
        return count;
    }
    int isPrime19(int n) {
        return n ==  2 || n ==  3 || n ==  5 ||  n == 7 ||
               n == 11 || n == 13 || n == 17 || n == 19;
    }
    int countPrimeSetBits(int L, int R) {
        // 因为 R 最大值是 10^6
        // 而且 log2(10^6) < 20
        // 所以 任意L~R数值 二进制位树最大是 19
        // 即可 判断L~R数值 的位数; 是否属于 [1, 19] 中的质数集合
        int ans = 0;
        for (int i = L; i <= R; ++i) {
            if (isPrime19(countBits(i)))
                ans++;
        }
        return ans;
    }
};