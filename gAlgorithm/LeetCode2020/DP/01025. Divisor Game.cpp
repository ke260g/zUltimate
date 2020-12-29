// Alice and Bob take turns playing a game, with Alice starting first.

// Initially, there is a number N on the chalkboard.  
// On each player's turn, that player makes a move consisting of:

//     Choosing any x with 0 < x < N and N % x == 0.
//     Replacing the number N on the chalkboard with N - x.

// Also, if a player cannot make a move, they lose the game.

// Return True if and only if Alice wins the game, assuming both players play optimally.

// Input: 2
// Output: true
// Explanation: Alice chooses 1, and Bob has no more moves.

// Input: 3
// Output: false
// Explanation: Alice chooses 1, Bob chooses 1, and Alice has no more moves.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/divisor-game
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    bool divisorGame(int N) {
        // 黑暗魔法; 100% ^_^ (自己推出来的 正确的)
        // 奇负偶胜 （老虎吃羊题?)
        // cause 如果当前数值N是奇数; 那么x无论怎么选; x必然是奇数
        // so    则下一轮选的人得到的 N-x 是偶数
        // 
        // cause N == 2 时; x选1; 则下家输
        // so    当偶数N让alice 选时; x选 1 即可; 不用想的
        //       从而使得 N-1 为奇数; 
        //       然后轮到Bob选; x只能时奇数;
        //       再到 alice; 因为 N-1-x 此时肯定时偶数; alice 故技重施
        return N & 1 ? false : true;

        // 正常思路(自己做的正确的)
        // N    ,  Alice
        // N:  1,  false  []
        // N:  2,  true   [1,]
        // N:  3,  false  [2,1]
        // N:  4,  true   [3,2,1] 但是如果 [2,1]
        // N:  5,  false  [4, ]
        // N:  6,  true   [5, ]   
        // Alice 选取一个x; 使得 record[N-x] = false 使得剩下树给Bob输
        // record[N] 表示当给定数字N-1由一个人选; 要么能赢要么输
        //
        // vector<bool> record(N, false);
        // for (int n = 2; n <= N; ++n) {
        //     for(int x = n-1; x >= 1; --x) {
        //         if ((n % x == 0) && !record[(n-1) - x]) {
        //             record[(n-1)] = true;
        //             break;
        //         }
        //     }
        //     // record[i] 没法选赢的 那就是输的
        // }
        // return record[N-1];
    }
};