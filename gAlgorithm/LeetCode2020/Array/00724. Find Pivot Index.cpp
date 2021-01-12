/*
 * @lc app=leetcode id=724 lang=cpp
 *
 * [724] Find Pivot Index
 *
 * https://leetcode.com/problems/find-pivot-index/description/
 *
 * algorithms
 * Easy (44.97%)
 * Total Accepted:    171.7K
 * Total Submissions: 381.9K
 * Testcase Example:  '[1,7,3,6,5,6]'
 *
 * Given an array of integers nums, write a method that returns the "pivot"
 * index of this array.
 * 
 * We define the pivot index as the index where the sum of all the numbers to
 * the left of the index is equal to the sum of all the numbers to the right of
 * the index.
 * 
 * If no such index exists, we should return -1. If there are multiple pivot
 * indexes, you should return the left-most pivot index.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,7,3,6,5,6]
 * Output: 3
 * Explanation:
 * The sum of the numbers to the left of index 3 (nums[3] = 6) is equal to the
 * sum of numbers to the right of index 3.
 * Also, 3 is the first index where this occurs.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1,2,3]
 * Output: -1
 * Explanation:
 * There is no index that satisfies the conditions in the problem
 * statement.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The length of nums will be in the range [0, 10000].
 * Each element nums[i] will be an integer in the range [-1000, 1000].
 * 
 * 
 */
// 自己想的
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        // 1. 求前缀和
        // 2. 遍历每个节点的左右和
        // 3. 如果相等则返回答案
        for (int i = 1; i < nums.size(); ++i)
            nums[i] += nums[i-1];
        for (int i = 0; i < nums.size(); ++i) {
            int l = i == 0 ? 0 : nums[i-1];
            int r = i == nums.size()-1 ? 0 : nums.back()-nums[i];
            if (l == r)
                return i;
        }
        return -1;
    }
};

// 官网答案
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        // 1. 求数组和
        // 2. 维护 i-1 的和;
        // 3. 针对每个i;  左边和就是之前累加的
        //    右边和 = 数组和 - 自己 - 左边和
        int sums = 0;
        for (int i = 0; i < nums.size(); ++i)
            sums += nums[i];
        int left = 0;
        for (int i = 0; i < nums.size(); ++i) {
            if (left == sums - left - nums[i])
                return i;
            left += nums[i];
        }
        return -1;
    }
};