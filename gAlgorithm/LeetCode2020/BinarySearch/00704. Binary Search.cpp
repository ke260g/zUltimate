// Given a sorted (in ascending order) integer array nums of n elements and a target value,
// write a function to search target in nums.
// If target exists, then return its index, otherwise return -1.

// Input: nums = [-1,0,3,5,9,12], target = 9
// Output: 4
// Explanation: 9 exists in nums and its index is 4

// Input: nums = [-1,0,3,5,9,12], target = 2
// Output: -1
// Explanation: 2 does not exist in nums so return -1

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/binary-search
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 最原始的 二分查找
class Solution {
public:
    int search(vector<int>& nums, int target) {
        if (nums.empty()) return -1;
        int l = 0, r = nums.size()-1;
        while (l <= r) {
            int m = l + (r-l)/2;
            if (nums[m] == target)
                return m;
            else if (nums[m] < target)
                l = m+1;
            else
                r = m-1;
        }
        return -1;
    }
};