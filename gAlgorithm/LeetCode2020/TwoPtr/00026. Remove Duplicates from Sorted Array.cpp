// Given a sorted array nums, remove the duplicates in-place 
// such that each element appears only once and returns the new length.

// Do not allocate extra space for another array, you must do this
// by modifying the input array in-place with O(1) extra memory.

// Example 1:
// Given nums = [1,1,2],
// Your function should return length = 2, with the first two elements of nums being 1 and 2 respectively.
// It doesn't matter what you leave beyond the returned length.

// Example 2:
// Given nums = [0,0,1,1,1,2,2,3,3,4],
// Your function should return length = 5, with the first
// five elements of nums being modified to 0, 1, 2, 3, and 4 respectively.
// It doesn't matter what values are set beyond the returned length.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/remove-duplicates-from-sorted-array
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 实际上这个算法就是用于  命令行工具 uniq 的; 还有c++的 std::list::unique
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        // idx:[0, uniq) // 已经单值化的序列
        // idx:[uniq, i)
        if (nums.empty()) return 0;
        int uniq = 1, i = 1;
        while (i < nums.size()) {
            if (nums[i] == nums[uniq-1])
                i++;
            else
                swap(nums[i++], nums[uniq++]);
        }
        return uniq;
    }
};