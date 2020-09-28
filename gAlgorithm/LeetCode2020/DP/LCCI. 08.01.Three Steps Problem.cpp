class Solution {
public:
    int waysToStep(int n) {
        const int module = 1000000007;
        if (n < 0) return 0;
        if (n <= 2) return n;
        if (n == 3) return 4; // 4种 [1,1,1] [1,2] [2,1] [3]
        // 1: 1
        // 2: 2
        // 3: 4
        // 4: 7
        // 5: 13
        // return waysToStep(n-1) + waysToStep(n-2) + waysToStep(n-3);
        // 斐波那数列
        int res = 0; int curr_step = 4;
        int prev[3] = { 1, 2, 4 }; // 斐波那窗口
        while(curr_step <= n) {
            res = (prev[0] + prev[1]) % module;
            res = (res + prev[2]) % module;
            // 右移动 斐波那数列窗口
            prev[0] = prev[1];
            prev[1] = prev[2];
            prev[2] = res;
            curr_step++;
        }
        return res;
    }
};