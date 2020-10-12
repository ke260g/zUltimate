// You are a professional robber planning to rob houses along a street. 
// Each house has a certain amount of money stashed. A
// ll houses at this place are arranged in a circle.  <<<---------- 区别于 00198的地方
// That means the first house is the neighbor of the last one. 
// Meanwhile, adjacent houses have a security system connected, 
// and it will automatically contact the police if two adjacent houses were broken into on the same night.

// Given a list of non-negative integers nums representing the amount of money of each house, 
// return the maximum amount of money you can rob tonight without alerting the police.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/house-robber-ii
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 00198 打劫问题的升级版; 这里地房子是构成环状的
// 有3种场景:
// 场景1: 开头可偷; 末尾不偷
// 场景2: 开头不偷; 末尾可偷
// 场景3: 开头末尾都不偷窃
// 3个场景下分别求 线性不成环条件下的最大价值; 然后3者取最大值
class Solution {
private:
    // 完全等同 198 问题
    int robCore(vector<int>& nums, int start, int stop) {
        if (start > stop) return 0;
        if (stop - start == 0) return nums[start];
        if (stop - start == 1) return max(nums[start], nums[stop]);
        if (stop - start == 2) return max(nums[start+1], nums[start]+nums[stop]);
        int prev[3] = { nums[start], nums[start+1], nums[start]+nums[start+2] };
        for (int i = start+3; i <= stop; ++i) {
            int curr = max(prev[0], prev[1]) + nums[i];
            prev[0] = prev[1];
            prev[1] = prev[2];
            prev[2] = curr;
        }
        return max(prev[1], prev[2]);
    }
public:
    int rob(vector<int>& nums) {
        // 场景1: 开头可偷 末尾不偷; 求 [0 ~ size-2] 区间的无环情况的最大
        // 场景2: 开头不偷 末尾可偷; 求 [1 ~ size-1] 区间的无环情况的最大
        // 场景3: 开头末尾都不偷;    求 [1 ~ size-2] 区间的无环情况的最大

        // 无环问题: f(n) = max(f(n-2), f(n-3)) + nums[n]
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        if (nums.size() == 2) return max(nums[0], nums[1]);

        int scene1 = robNoRing(nums, 0, nums.size()-2);
        int scene2 = robNoRing(nums, 1, nums.size()-1);
        int scene3 = robNoRing(nums, 1, nums.size()-2);

        return max(scene1, max(scene2, scene3));
    }
};