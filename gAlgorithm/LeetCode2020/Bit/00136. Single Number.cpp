// Given a non-empty array of integers nums, every element appears twice except for one. 
// Find that single one.

// Follow up: Could you implement a solution with a linear runtime complexity and without using extra memory?

// Input: nums = [2,2,1]
// Output: 1

// Input: nums = [4,1,2,1,2]
// Output: 4

// Input: nums = [1]
// Output: 1

// 1, 全部数值 直接 异或 操作
// 2. 异或运算 性质:
// 2.1 a ^ 0 == 0
// 2.2 a ^ a == 0
// 2.3 满足结合律和交换律 b^a^b == a^b^b == a^(b^b)
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ans = 0;
        for (auto num: nums)
            ans ^= num;
        return ans;
    }
};