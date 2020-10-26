// Given an integer array nums of 2n integers,
// group these integers into n pairs of integer,
// say (a1, b1), (a2, b2), ..., (an, bn) 
// which makes the sum of min(ai, bi) for all i from 1 to n 
// as large as possible.

// Input: nums = [1,4,3,2]
// Output: 4
// Explanation: n is 2, and the maximum sum of pairs is 4 = min(1, 2) + min(3, 4).

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/array-partition-i
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 如题; 要让 min(a, b) 总和最大; 显然是要把 (a,b) 的差最小
// 因为 b 超出 a 的值会被 a 吞掉
// 先排序; 两两配对即可
class Solution {
public:
    int arrayPairSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int res = 0;
        for (int i = 1; i < nums.size(); i += 2)
            res += min(nums[i-1], nums[i]);
        return res;
    }
};