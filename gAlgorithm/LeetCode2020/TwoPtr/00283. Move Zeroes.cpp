// Given an array nums, write a function to move all 0's to the end of it while maintaining 
// the relative order of the non-zero elements.

// Input: [0,1,0,3,12]
// Output: [1,3,12,0,0]

// Note:
//     You must do this in-place without making a copy of the array.
//     Minimize the total number of operations.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/move-zeroes
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 题目要求: 双 O(1) 复杂度
// 00027 Remove Element 的特殊情况 算法思路完全一样
class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        // 双O(1) 复杂度
        // idx: [0, zero)   指示已排序
        // idx: [zero, i)   指示0
        int zero = 0, i = 0;
        while (i < nums.size()) {
            if (nums[i] == 0)
                ++i;
            else
                swap(nums[i++], nums[zero++]);
        }
        return;
    }
};