class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        if (cost.size() == 0) return 0;
        if (cost.size() == 1) return 0; // 直接跳过
        if (cost.size() == 2) return min(cost[0], cost[1]);

        // 自己想的正确方法
        // A - B - C (三阶里面必须走两阶)
        // 走法0:  k-2 -> k # A C  跳过上一阶       
        // 走法1:  k-1 -> k #  BC  走上一阶; 也走自己这一阶
        // 走法2:  k-1 ->   # AB   不走自己这一阶
        //
        // 实际上; 对于D 来说
        // A - B - C - D
        // 走法0:  B D  @ 有两种方法 [A BD, _BD]
        // 走法1:  C D  @ 有两种方法 [ACD, BCD]
        // 走法2:  B C
        // 所以D在走法1 需要确定 AC 和 BC 消耗较少的那个
        // 同理D在走法0 实际上也 需要确定 AB _B 哪个消耗更少
        //
        // vector<vector<int>> prev_cost(2, vector<int>(3/*3种走法*/, 0));
        // prev_cost[0][0] = 0 + cost[0];  // 走法0
        // prev_cost[0][2] = 0;            // 走法2
        // prev_cost[0][1] = prev_cost[0][2] + cost[0]; // 走法1
        //
        // prev_cost[1][0] = 0 + cost[1];  // 走法0
        // prev_cost[1][2] = min(prev_cost[0][0], prev_cost[0][1]);
        // prev_cost[1][1] = prev_cost[1][2] + cost[1]; // 走法1
        //
        // for (int i = 2; i < cost.size(); ++i) {
        //     int step_type0 = min(prev_cost[0][0],  prev_cost[0][1]) + cost[i];
        //     int step_type2 = min(prev_cost[1][0], prev_cost[1][1]);
        //     int step_type1 = step_type2 + cost[i];
        //     prev_cost[0][0] = prev_cost[1][0];
        //     prev_cost[0][1] = prev_cost[1][1];
        //     prev_cost[0][2] = prev_cost[1][2];
        //     prev_cost[1][0] = step_type0;
        //     prev_cost[1][1] = step_type1;
        //     prev_cost[1][2] = step_type2;
        // }
        // return min(prev_cost[1][0], min(prev_cost[1][1], prev_cost[1][2]));

        // 下面是官方题解; 思路是一样的; 但是实现起来更优
        // f(k) 表示如果"踏"这一阶的花费
        // 而"踏"k阶的路线: 要么"踏"k-1阶; 要么"踏"k-2阶
        // 所以 f(k) = min(f(k-1), f(k-1)) + cost[i]
        // 走完k阶的实际花费就是 "踏"k阶 or "踏"k-1阶
        vector<int> prev_cost(2, 0);
        for (int i = 0; i < cost.size(); i++) {
            int curr_cost = min(prev_cost[0], prev_cost[1]) + cost[i];
            prev_cost[0] = prev_cost[1];
            prev_cost[1] = curr_cost;
        }
        return min(prev_cost[0], prev_cost[1]);
    }
};