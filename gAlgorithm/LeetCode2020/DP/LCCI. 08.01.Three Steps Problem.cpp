// 最后一级必须踩上
class Solution {
public:
    int waysToStep(int n) {
        // f(k) 表示走当前阶的走法
        // 要么 f(k-3) 上上上阶再走一步        
        // 要么 f(k-2)   上上阶再走一步
        // 要么 f(k-1) 　  上阶再走两步
        // 所以 f(k) = f(k-2) + f(k-1) + f(k-3) 

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
        // 斐波那契数列的变种
        int res = 0; int curr_step = 4;
        int prev[3] = { 1, 2, 4 }; // 斐波那窗口
        while(curr_step <= n) {
            res = (prev[0] + prev[1]) % module;
            res = (res + prev[2]) % module;
            // 右移动 斐波那数列窗口
            memmove(prev, prev+1, 2*sizeof(int));
            prev[2] = res;
            curr_step++;
        }
        return res;
    }
};