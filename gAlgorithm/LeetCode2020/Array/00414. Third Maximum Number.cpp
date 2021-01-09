/*
 * @lc app=leetcode id=414 lang=cpp
 *
 * [414] Third Maximum Number
 *
 * https://leetcode.com/problems/third-maximum-number/description/
 *
 * algorithms
 * Easy (30.57%)
 * Total Accepted:    192.5K
 * Total Submissions: 629.6K
 * Testcase Example:  '[3,2,1]'
 *
 * Given integer array nums, return the third maximum number in this array. If
 * the third maximum does not exist, return the maximum number.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [3,2,1]
 * Output: 1
 * Explanation: The third maximum is 1.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1,2]
 * Output: 2
 * Explanation: The third maximum does not exist, so the maximum (2) is
 * returned instead.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: nums = [2,2,3,1]
 * Output: 1
 * Explanation: Note that the third maximum here means the third maximum
 * distinct number.
 * Both numbers with value 2 are both considered as second maximum.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= nums.length <= 104
 * 2^31 <= nums[i] <= 2^31 - 1
 * 
 * 
 * 
 * Follow up: Can you find an O(n) solution?
 */
class Solution {
public:
    int thirdMax(vector<int>& nums) {
        // 1. 空数组
        // 2. 动态维护 最大的3个值 mmax数组
        // 3. 当数组不够3个; 如果不在 mmax数组中
        //    push_back 后直接 sort
        //    成员数量 < 3; sort 是常量复杂度
        // 4. 当mmax数组成员 == 3
        //    找到 nums[i] 在 mmax[i] 的插入位置 再插入
        //    插入位置有3种场景 if-elif-elif 即可
        // Note:
        //    等同 00628. Maximum Product of Three Numbers.cpp
        //    中用到的 动态维护最大3个值的 方法
        if (nums.empty()) return 0;
        vector<int> mmax; // 降序
        for (int i = 0; i < nums.size(); i++) {
            if (find(mmax.begin(), mmax.end(), nums[i]) != mmax.end())
                continue;
            if (mmax.size() < 3) {
                if (find(mmax.begin(), mmax.end(), nums[i]) == mmax.end()) {
                    mmax.push_back(nums[i]);
                    sort(mmax.begin(), mmax.end(), greater<int>());
                }
            } else {
                if (nums[i] > mmax[0]) {
                    mmax[2] = mmax[1];
                    mmax[1] = mmax[0];
                    mmax[0] = nums[i];
                } else if (mmax[0] > nums[i] && nums[i] > mmax[1]) {
                    mmax[2] = mmax[1];
                    mmax[1] = nums[i];
                } else if (mmax[1] > nums[i] && nums[i] > mmax[2]) {
                    mmax[2] = nums[i]; 
                }
            }
        }
        return mmax.size() == 3 ? mmax.back() : mmax.front();
    }
};
