// We are playing the Guess Game. The game is as follows:

// I pick a number from 1 to n. You have to guess which number I picked.

// Every time you guess wrong, I will tell you whether the number I picked is higher or lower than your guess.

// You call a pre-defined API int guess(int num), which returns 3 possible results:

//     -1: The number I picked is lower than your guess (i.e. pick < num).
//     1: The number I picked is higher than your guess (i.e. pick > num).
//     0: The number I picked is equal to your guess (i.e. pick == num).

// Input: n = 10, pick = 6
// Output: 6

// Input: n = 2, pick = 1
// Output: 1

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/guess-number-higher-or-lower
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 原汁原味的二分查找问题
/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is lower than the guess number
 *			      1 if num is higher than the guess number
 *               otherwise return 0
 * int guess(int num);
 */
class Solution {
public:
    int guessNumber(int n) {
        int l = 1, r = n;
        while (l <= r) {
            int m = l + (r-l)/2;
            if (guess(m) == 0)
                return m;
            else if (guess(m) > 0)
                l = m + 1;
            else {
                r = m - 1;
            }
                
        }
        return 0;
    }
};