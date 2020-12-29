//  On a staircase, the i-th step has some non-negative cost cost[i] assigned (0 indexed).

// Once you pay the cost, you can either climb one or two steps. 
// You need to find minimum cost to reach the top of the floor, 
// and you can either start from the step with index 0, or the step with index 1. 

// Input: cost = [10, 15, 20]
// Output: 15
// Explanation: Cheapest is start on cost[1], pay that cost and go to the top.

// Input: cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1]
// Output: 6
// Explanation: Cheapest is start on cost[0], and only step on 1s, skipping cost[3].

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/min-cost-climbing-stairs
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        if (cost.size() < 2) return 0; // 直接跳过
        if (cost.size() == 2) return min(cost[0], cost[1]);

        // 下面是官方题解; 思路是一样的; 但是实现起来更优
        // f(k) 表示如果"踏"这一阶的花费
        // 而"踏"k阶的路线: 要么"踏"k-1阶; 要么"踏"k-2阶
        // 所以 f(k) = min(f(k-1), f(k-1)) + cost[i]
        // 走完k阶的实际花费就是 "踏"k阶 or "踏"k-1阶
        int take[2] = { cost[0], cost[1] };
        for (int i = 2; i < cost.size(); i++) {
            int curr = min(take[0], take[1]) + cost[i];
            take[0] = take[1];
            take[1] = curr;
        }
        return min(take[0], take[1]);
    }
};