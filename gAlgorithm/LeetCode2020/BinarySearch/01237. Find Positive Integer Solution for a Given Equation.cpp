// Given a function  f(x, y) and a value z, return all positive integer pairs x and y where f(x,y) == z.

// The function is constantly increasing, i.e.:

//     f(x, y) < f(x + 1, y)
//     f(x, y) < f(x, y + 1)


// For custom testing purposes you're given an integer function_id and a target z as input, 
// where function_id represent one function from an secret internal list, 
// on the examples you'll know only two functions from the list.  

// Input: function_id = 1, z = 5
// Output: [[1,4],[2,3],[3,2],[4,1]]
// Explanation: function_id = 1 means that f(x, y) = x + y

// Input: function_id = 2, z = 5
// Output: [[1,5],[5,1]]
// Explanation: function_id = 2 means that f(x, y) = x * y

// Constraints:
// 1 <= function_id <= 9
// 1 <= z <= 100
// It's guaranteed that the solutions of f(x, y) == z will be on the range 1 <= x, y <= 1000
// It's also guaranteed that f(x, y) will fit in 32 bit signed integer if 1 <= x, y <= 1000

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/find-positive-integer-solution-for-a-given-equation
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 题目用人话翻译一下
// 1. 给定二元函数 z = f(x, y)，二元单调
// 2. 给定 z, 求出满足条件的坐标
// 3. 限制: x 值域 [1, 1000]; y 值域 [1, 1000]
// 
// 实际上 这个题目涉及一个数学定理:
// 单调递增的二元函数中
// 给定 z 和 x, 如果存在满足条件的 z = f(x, y), 那么 y 有且只有 1 个
// 给定 z 和 y, 如果存在满足条件的 z = f(x, y), 那么 x 有且只有 1 个
// 
// 优化:
// 根据单调递增原理
// 如果 f(x, y) == z; 
// 那么 f(x, y+n) > z  and  f(x+n, y) > z
// 所以 f(x, y) == z 之后; 
// 只需要判定 f(x+n, y-1) 即可

/*
 * // This is the custom function interface.
 * // You should not implement it, or speculate about its implementation
 * class CustomFunction {
 * public:
 *     // Returns f(x, y) for any given positive integers x and y.
 *     // Note that f(x, y) is increasing with respect to both x and y.
 *     // i.e. f(x, y) < f(x + 1, y), f(x, y) < f(x, y + 1)
 *     int f(int x, int y);
 * };
 */
class Solution {
public:
    vector<vector<int>> findSolution(CustomFunction& customfunction, int z) {
        vector<vector<int>> answer;
        int right = 1000; // 看了答案的优化
        for (int x = 1; x <= 1000; ++x) {
            int l = 1;
            // int r = 1000;
            int r = right; // 看了答案的优化
            while (l <= r) {
                int m = l + (r-l)/2;
                int d = customfunction.f(x, m);
                if (d == z) {
                    vector<int> ans(2, 0);
                    ans[0] = x;
                    ans[1] = m;
                    right = m - 1; // 看了答案的优化
                    answer.push_back(ans);
                    break;
                } else if (d < z) {
                    l = m + 1;
                } else {
                    r = m - 1;
                }
            }
        }
        return answer;
    }
};