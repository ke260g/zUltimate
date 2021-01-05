// A majority element is an element that makes up more than half of the items in an array.
// Given a positive integers array, find the majority element.
// If there is no majority element, return -1. Do this in O(N) time and O(1) space.

// Input: [1,2,5,9,5,9,5,5,5]
// Output: 5

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/find-majority-element-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 摩尔投票法
// 投票法求得的数 满足 必要不充分条件; 因为多个相同次数的算不出来
// 比如 [1,2,3]；这个Input应该返回 -1; 但实际上却返回了 3
// 所以还得补一轮验证
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int candicate = 0, count = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (count == 0)
                candicate = nums[i];
            count += candicate == nums[i] ? 1 : -1;
        }
        count = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (candicate == nums[i])
                count++;
        }
        return count > nums.size()/2 ? candicate : -1;
    }
};