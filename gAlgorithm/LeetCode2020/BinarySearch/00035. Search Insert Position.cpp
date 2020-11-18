// Given a sorted array of distinct integers and a target value,
// return the index if the target is found. If not,
// return the index where it would be if it were inserted in order.

// Input: nums = [1,3,5,6], target = 5
// Output: 2

// Input: nums = [1,3,5,6], target = 2
// Output: 1

// Input: nums = [1,3,5,6], target = 7
// Output: 4

// Input: nums = [1,3,5,6], target = 0
// Output: 0

// Input: nums = [1], target = 0
// Output: 0

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/search-insert-position
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 坑1: 超出范围的; 不能用于二分查找; 要提前判断
// 坑2: 当踩到中间值时,
//      nums[m == l == r] < target, 那么 ans = m+1, 即最后的中间值的右边
//      nums[m == l == r] > target, 那么 ans = m, 把最后中间值右移, target替换
//      实际上; ans 的赋值; 只有最后 l == r 才有意义
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int l = 0, r = nums.size()-1;
        int ans = 0;
        if (target < nums[0])
            return 0;
        if (target > nums[nums.size()-1])
            return nums.size();
        while (l <= r) {
            int m = l + (r-l) / 2;
            if (nums[m] == target)
                return m;
            else if (nums[m] < target) {
                l = m + 1;
                ans = l;
            } else {
                r = m - 1;
                ans = m;
            }
        }
        return ans;
    }

};