/*
 * @lc app=leetcode id=448 lang=cpp
 *
 * [448] Find All Numbers Disappeared in an Array
 *
 * https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array/description/
 *
 * algorithms
 * Easy (56.09%)
 * Total Accepted:    326.1K
 * Total Submissions: 581.5K
 * Testcase Example:  '[4,3,2,7,8,2,3,1]'
 *
 * Given an array of integers where 1 ≤ a[i] ≤ n (n = size of array), some
 * elements appear twice and others appear once.
 * 
 * Find all the elements of [1, n] inclusive that do not appear in this array.
 * 
 * Could you do it without extra space and in O(n) runtime? You may assume the
 * returned list does not count as extra space.
 * 
 * Example:
 * 
 * Input:
 * [4,3,2,7,8,2,3,1]
 * 
 * Output:
 * [5,6]
 * 
 * 
 */

class Solution {
public:
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        vector<int> ans;
#if 1 // 使用额外空间; 记录出现过的数字; [ time = O(n+m), space = O(n) ]
        vector<int> full(nums.size(), 0);
        for (auto n:nums)
            full[n-1] = 1;
        for (int i = 0; i < full.size(); ++i) {
            if (full[i] == 0)
                ans.push_back(i+1);
        }
#endif
#if 0 // 不适用额外空间; [ time = O(n), space = O(1) ]
        int i = 0;
        // 思路: 把 value 放到对应索引的位置 nums[value-1] = value
        // 遍历整个数组; 有三种情况
        // 1. value 已经放到 正确的位置了
        // 2. value 没有放到正确的位置; 但 正确的位置上 == value
        //    说明 value 是重复的; 遍历下一个元素; i++
        // 3. value 没有放到正确的位置; 正确位置上不是 value
        //    那就把 value 放到正确的位置; (用 swap)
        //    继续遍历当前位置; i 不变
        // 4. 再次遍历nums[]; 如果 位置上的value不正确
        //    那么 位置上对应的值 就是消失不见的
        while (i < nums.size()) {
            if (nums[i]-1 == i) {
                ++i;
                continue;
            }
            if (nums[nums[i]-1] == nums[i]) {
                ++i;
            } else {
                swap(nums[nums[i]-1], nums[i]);
            }
        }
        for (int i = 0; i < nums.size(); i++) {
            if (i != nums[i]-1)
                ans.push_back(i+1);
        } 
#endif
        return ans;
    }
};
// Note: 使用额外空间的版本; 运行效率要快一点
