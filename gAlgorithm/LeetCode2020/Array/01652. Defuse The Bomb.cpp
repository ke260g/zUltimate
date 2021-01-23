/*
 * @lc app=leetcode id=1652 lang=cpp
 *
 * [1652] Defuse the Bomb
 *
 * https://leetcode.com/problems/defuse-the-bomb/description/
 *
 * algorithms
 * Easy (64.69%)
 * Total Accepted:    9.8K
 * Total Submissions: 15.3K
 * Testcase Example:  '[5,7,1,4]\n3'
 *
 * You have a bomb to defuse, and your time is running out! Your informer will
 * provide you with a circular array code of length of n and a key k.
 * 
 * To decrypt the code, you must replace every number. All the numbers are
 * replaced simultaneously.
 * 
 * 
 * If k > 0, replace the i^th number with the sum of the next k numbers.
 * If k < 0, replace the i^th number with the sum of the previous k
 * numbers.
 * If k == 0, replace the i^th number with 0.
 * 
 * 
 * As code is circular, the next element of code[n-1] is code[0], and the
 * previous element of code[0] is code[n-1].
 * 
 * Given the circular array code and an integer key k, return the decrypted
 * code to defuse the bomb!
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: code = [5,7,1,4], k = 3
 * Output: [12,10,16,13]
 * Explanation: Each number is replaced by the sum of the next 3 numbers. The
 * decrypted code is [7+1+4, 1+4+5, 4+5+7, 5+7+1]. Notice that the numbers wrap
 * around.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: code = [1,2,3,4], k = 0
 * Output: [0,0,0,0]
 * Explanation: When k is zero, the numbers are replaced by 0. 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: code = [2,4,9,3], k = -2
 * Output: [12,5,6,13]
 * Explanation: The decrypted code is [3+9, 2+3, 4+2, 9+4]. Notice that the
 * numbers wrap around again. If k is negative, the sum is of the previous
 * numbers.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * n == code.length
 * 1 <= n <= 100
 * 1 <= code[i] <= 100
 * -(n - 1) <= k <= n - 1
 * 
 * 
 */
class Solution {
public:
    vector<int> decrypt(vector<int>& code, int k) {
        vector<int> answer(code.size(), 0);
        if (k < 0) {
            // 1. 首先 取得 末尾k个的和
            // 2. 记录前置窗口的开始
            // 3. 从0开始遍历每个 code
            // 4. 变换后的值 即 窗口值
            // 5. 为下一个 code 准备window:
            //    干掉window的头; window头指向下一位
            //    window 再加上当前code
            int head = code.size() + k;
            int window = 0;
            for (int i = head; i < code.size(); i++)
                window += code[i];
            for (int i = 0; i < code.size(); i++) {
                answer[i] = window;
                window = window - code[head++ % code.size()] + code[i];
            }
        } else if (k > 0) {
            // 1. 首先取得 后面k个的和
            // 2. 记录后续窗口的末尾
            // 3. 从0开始遍历每个code
            // 4. 变换后的值 即 窗口值
            // 5. 为下一个 code 准备 window:
            //    干掉window的头(即 当前code的下一个code)
            //    再加上 window尾部 后的一个code
            //    后续窗口的末尾++
            int tail = k;
            int window = 0;
            for (int i = 1; i <= tail; ++i)
                window += code[i];
            for (int i = 0; i < code.size(); ++i) {
                answer[i] = window;
                window = window - code[(i+1) % code.size()] + code[++tail % code.size()];
            }
        }
        return answer;
    }
};
