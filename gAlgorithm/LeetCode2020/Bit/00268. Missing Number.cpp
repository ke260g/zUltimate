// Given an array nums containing n distinct numbers in the range [0, n], 
// return the only number in the range that is missing from the array.

// Follow up: Could you implement a solution using only O(1) extra space complexity and O(n) runtime complexity?

// Input: nums = [3,0,1]
// Output: 2
// Explanation: n = 3 since there are 3 numbers, so all numbers are in the range [0,3]. 
// 2 is the missing number in the range since it does not appear in nums.

// Input: nums = [9,6,4,2,3,5,7,0,1]
// Output: 8
// Explanation: n = 9 since there are 9 numbers, so all numbers are in the range [0,9]. 8 is the missing number in the range since it does not appear in nums.

// Input: nums = [0]
// Output: 1
// Explanation: n = 1 since there is 1 number, so all numbers are in the range [0,1]. 1 is the missing number in the range since it does not appear in nums.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/missing-number
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 求尺寸为 n 的数组中; 理论上 [1,2,3,...,n] 的数列中一个数值变成了0
// 求得那个数值


// 方法1: 利用 00136. Single Number 的方法
// 任意数 ^ 数值1 ^ 数值1 == 任意数
// 所以 给定数组的元素 逐个xor ; 再 xor 没有missing的数组
//      那么求得missing 的数值
// nums[] ^ [1,2,...,nums.size()]
//
// 这里有坑: 因为我们用i 遍历 nums[];
//           i 的取值 [0, nums.size()-1],
//           所以计算时需要 n+1
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = 0;
        for (int i = 0; i < nums.size(); ++i) {
            n ^= (i+1) ^ nums[i];
        }
        return n;
    }
};

// 方法2: 高斯求和; 求的 没有missing的数组和
//        减去给定数组的和
class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int n = (1+nums.size())*nums.size() / 2;
        for (auto num: nums)
            n -= num;
        return n;
    }
};