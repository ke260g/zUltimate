/*
 * @lc app=leetcode id=665 lang=cpp
 *
 * [665] Non-decreasing Array
 *
 * https://leetcode.com/problems/non-decreasing-array/description/
 *
 * algorithms
 * Easy (19.63%)
 * Total Accepted:    116.8K
 * Total Submissions: 594.8K
 * Testcase Example:  '[4,2,3]'
 *
 * Given an array nums with n integers, your task is to check if it could
 * become non-decreasing by modifying at most 1 element.
 * 
 * We define an array is non-decreasing if nums[i] <= nums[i + 1] holds for
 * every i (0-based) such that (0 <= i <= n - 2).
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [4,2,3]
 * Output: true
 * Explanation: You could modify the first 4 to 1 to get a non-decreasing
 * array.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [4,2,1]
 * Output: false
 * Explanation: You can't get a non-decreasing array by modify at most one
 * element.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= n <= 10 ^ 4
 * - 10 ^ 5 <= nums[i] <= 10 ^ 5
 * 
 * 
 */
class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        // 1. 找到第一个递减点后; 尝试修改并维持正确的非递减序列
        //    要么修改 nums[i-1] = nums[i]
        //    要么修改 nums[i] = nums[i-1]
        //    因为 i+1 是不确定的; 我们尽可能的避免修改 nums[i]
        //
        // 1.1 nums[i-1]  = nums[i] 后能变成非递减序列条件
        // 1.1.1  i == 1
        // 1.1.2  nums[i-2] <= nums[i]
        // 1.2 其他场景只能 nums[i] = nums[i-1]
        //
        // 2. 如果后续不再有递减点; 满足条件
        // 3. 如果后续找到第二个递减点; 不满足
        bool decrease = false;
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] < nums[i-1]) {
                if (decrease)
                    return false;
                else {
                    if (i == 1 || nums[i-2] <= nums[i])
                        nums[i-1] = nums[i];
                    else
                        nums[i] = nums[i-1];
                    decrease = true;
                }
            }
        }
        return true;
    }
};
