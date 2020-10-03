// You are a professional robber planning to rob houses along a street. 
// Each house has a certain amount of money stashed, 
// the only constraint stopping you from robbing each of them is that adjacent houses have security system connected
//  and it will automatically contact the police if two adjacent houses were broken into on the same night.
//
// Given a list of non-negative integers representing the amount of money of each house, 
// determine the maximum amount of money you can rob tonight without alerting the police.
//
// Input: nums = [1,2,3,1]
// Output: 4
// Explanation: [1, 3]
//
// Input: nums = [2,7,9,3,1]
// Output: 12
// Explanation: [2,9,1]
//
// Input: nums = [2, 1, 1, 2]
// Output: 4
// Explanation: [2,2]

class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        if (nums.size() == 2) return max(nums[0], nums[1]);
        if (nums.size() == 3) return max(nums[1], nums[0] + nums[2]);
        // rob_val[i] 表示如果盗窃了 i 号房子所得最大金钱数
        // 由于相邻房子不能同时被访问
        // 所以如果 i 号房子被偷窃; 有且只有两种情况
        // 1. i-3 ~ i
        // 2. i-2 ~ i
        // 如果存在第3种; 那么这种就是 i-4 ~ i
        //    但是 这种情况下; 一定可以去一趟i-2房子的
        //    所以 退化成 i-4 ~ i-2 ~ i 即情况2
        vector<int> rob_val(nums.size(), 0);
        rob_val[0] = nums[0];
        rob_val[1] = nums[1];
        rob_val[2] = nums[0] + nums[2];
        for(int i = 3; i < nums.size(); ++i)
            rob_val[i] = max(rob_val[i-3], rob_val[i-2]) + nums[i];
        return max(rob_val[nums.size()-1], rob_val[nums.size()-2]);
    }
};