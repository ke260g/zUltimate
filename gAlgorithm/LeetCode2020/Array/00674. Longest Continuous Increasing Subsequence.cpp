/*
 * @lc app=leetcode id=674 lang=cpp
 *
 * [674] Longest Continuous Increasing Subsequence
 *
 * https://leetcode.com/problems/longest-continuous-increasing-subsequence/description/
 *
 * algorithms
 * Easy (46.01%)
 * Total Accepted:    133.3K
 * Total Submissions: 289.6K
 * Testcase Example:  '[1,3,5,4,7]'
 *
 * Given an unsorted array of integers nums, return the length of the longest
 * continuous increasing subsequence (i.e. subarray). The subsequence must be
 * strictly increasing.
 * 
 * A continuous increasing subsequence is defined by two indices l and r (l <
 * r) such that it is [nums[l], nums[l + 1], ..., nums[r - 1], nums[r]] and for
 * each l <= i < r, nums[i] < nums[i + 1].
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,3,5,4,7]
 * Output: 3
 * Explanation: The longest continuous increasing subsequence is [1,3,5] with
 * length 3.
 * Even though [1,3,5,7] is an increasing subsequence, it is not continuous as
 * elements 5 and 7 are separated by element
 * 4.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [2,2,2,2,2]
 * Output: 1
 * Explanation: The longest continuous increasing subsequence is [2] with
 * length 1. Note that it must be strictly
 * increasing.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= nums.length <= 10^4
 * -10^9 <= nums[i] <= 10^9
 * 
 * 
 */
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        // 1. 记录当前正在上升 的序列长度
        // 2. 如果 curr <= prev; 没有上升; 当前正在上升的序列长度是零
        // 3. 如果 curr >  prev; 正在上升; 当前正在上升的序列长度++
        // 4. 更新最长的上升序列长度
        if (nums.empty()) return 0;
        int candicate = 1, continuous = 1; // 首个数值 一定满足条件
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] > nums[i-1]) {
                continuous++;
                candicate = max(candicate, continuous);
            } else {
                continuous = 1; // 不用更新 candicate 
            }
        }
        return candicate;
    }
};
