// You are climbing a stair case. It takes n steps to reach to the top.
// Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

class Solution {
public:
    int climbStairs(int n) {
        if (n <= 0)
            return 0;
        if (n == 1)
            return 1;
        if (n == 2)
            return 2;
        // return climbStairs(n-1) + climbStairs(n-2); // 递归太慢了

        // 改用斐波那 数列实现
        int res = 0, prev[2] = { 1, 2 }, curr_step = 3;
        while (curr_step <= n) {
            res = prev[0] + prev[1];
            prev[0] = prev[1];  // 斐波那 数列上上个成员 = 上一个成员 (右移)
            prev[1] = res;      // 斐波那 数列上一个成员 = 结果      (右移)
            curr_step++;        // 走上了楼梯; 步进
        }
        return res;
    }
};
// 对比 LCCI. 08.01.Three Steps Problem