/*
 * @lc app=leetcode id=1608 lang=cpp
 *
 * [1608] Special Array With X Elements Greater Than or Equal X
 *
 * https://leetcode.com/problems/special-array-with-x-elements-greater-than-or-equal-x/description/
 *
 * algorithms
 * Easy (61.74%)
 * Total Accepted:    16.3K
 * Total Submissions: 26.4K
 * Testcase Example:  '[3,5]'
 *
 * You are given an array nums of non-negative integers. nums is considered
 * special if there exists a number x such that there are exactly x numbers in
 * nums that are greater than or equal to x.
 * 
 * Notice that x does not have to be an element in nums.
 * 
 * Return x if the array is special, otherwise, return -1. It can be proven
 * that if nums is special, the value for x is unique.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [3,5]
 * Output: 2
 * Explanation: There are 2 values (3 and 5) that are greater than or equal to
 * 2.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [0,0]
 * Output: -1
 * Explanation: No numbers fit the criteria for x.
 * If x = 0, there should be 0 numbers >= x, but there are 2.
 * If x = 1, there should be 1 number >= x, but there are 0.
 * If x = 2, there should be 2 numbers >= x, but there are 0.
 * x cannot be greater since there are only 2 numbers in nums.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [0,4,3,0,4]
 * Output: 3
 * Explanation: There are 3 values that are greater than or equal to 3.
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: nums = [3,6,7,7,0]
 * Output: -1
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 100
 * 0 <= nums[i] <= 1000
 * 
 * 
 */
class Solution {
public:
    int specialArray(vector<int>& nums) {
        // 原理:
        //   针对已排序的数组;
        //   如果有且只有 x 个数 大于等于x,
        //   那么数组拆分两半
        //   左边 全小于 x (nums[size-x-1] < x)
        //   右边 不小于 x (x <= nums[size-x])
        //
        //   再处理两个特殊情况;
        //   x == size; 左边没有数值
        //   x == 0;    右边没有数值

        sort(nums.begin(), nums.end());
        for (int i = 0; i <= nums.size(); ++i) {
            if ((i == nums.size() || nums[nums.size()-i-1] < i) &&
                (i == 0 || i <= nums[nums.size()-i]))
                return i;
        }
        return -1;
    }
};