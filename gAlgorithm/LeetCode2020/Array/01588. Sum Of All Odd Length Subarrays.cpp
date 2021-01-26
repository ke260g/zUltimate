/*
 * @lc app=leetcode id=1588 lang=cpp
 *
 * [1588] Sum of All Odd Length Subarrays
 *
 * https://leetcode.com/problems/sum-of-all-odd-length-subarrays/description/
 *
 * algorithms
 * Easy (81.76%)
 * Total Accepted:    30.2K
 * Total Submissions: 36.8K
 * Testcase Example:  '[1,4,2,5,3]'
 *
 * Given an array of positive integers arr, calculate the sum of all possible
 * odd-length subarrays.
 * 
 * A subarray is a contiguous subsequence of the array.
 * 
 * Return the sum of all odd-length subarrays of arr.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [1,4,2,5,3]
 * Output: 58
 * Explanation: The odd-length subarrays of arr and their sums are:
 * [1] = 1
 * [4] = 4
 * [2] = 2
 * [5] = 5
 * [3] = 3
 * [1,4,2] = 7
 * [4,2,5] = 11
 * [2,5,3] = 10
 * [1,4,2,5,3] = 15
 * If we add all these together we get 1 + 4 + 2 + 5 + 3 + 7 + 11 + 10 + 15 =
 * 58
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [1,2]
 * Output: 3
 * Explanation: There are only 2 subarrays of odd length, [1] and [2]. Their
 * sum is 3.
 * 
 * Example 3:
 * 
 * 
 * Input: arr = [10,11,12]
 * Output: 66
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 100
 * 1 <= arr[i] <= 1000
 * 
 * 
 */
class Solution {
public:
    int sumOddLengthSubarrays(vector<int>& arr) {
        int sum = 0;

        int row = 0;
        for (int i = 0; i < arr.size(); ++i)
            row += arr[i];

        // 减去两对角法
        // len = 1:
        //     [1,0,0,0,0,0]
        //     [0,2,0,0,0,0]
        //     [0,0,3,0,0,0]
        //     [0,0,0,4,0,0]
        //     [0,0,0,0,5,0]
        //     [0,0,0,0,0,6]
        //       transform
        //     [1,2,3,4,5,6]
        // len = 3:
        //     [1,2,3,0,0,0]
        //     [0,2,3,4,0,0]
        //     [0,0,3,4,5,0]
        //     [0,0,0,4,5,6]
        //       transform
        //     [1,2,3,4,0,0]
        //     [0,2,3,4,5,0]
        //     [0,0,3,4,5,6]
        // len = 5:
        //     [1,2,3,4,5,0]
        //     [0,2,3,4,5,6]
        //       transform
        //     [1,2,0,0,0,0]
        //     [0,2,3,0,0,0]
        //     [0,0,3,4,0,0]
        //     [0,0,0,4,5,0]
        //     [0,0,0,0,5,6]

#if 0 // (normal) 原始版本的 减去对角法
        for (int len = 1; len <= arr.size(); len += 2) {
            int sub = arr.size() - len + 1; // 子系列个数
            sum += sub * row;
            for (int i = 1; i < sub; ++i)
                sum -= arr[i-1]*(sub-i) + arr[arr.size()-i]*(sub-i);
        }
#endif

#if 1 // (fast) 减两边对角; transform 后的计算
        for (int len = 1; len <= arr.size(); len += 2) {
            sum += len * row;
            for (int i = 1; i < len; ++i)
                sum -= arr[i-1]*(len-i) + arr[arr.size()-i]*(len-i);
        }
#endif
#if 0 // (slow) 原始遍历法 (三层 循环; 贼慢)
        for (int len = 1; len <= arr.size(); len += 2) {
            for (int begin = 0; begin <= arr.size()-len; begin++) {
                for (int i = begin; i < begin+len; i++) {
                    sum += arr[i];
                }
            }
        }
#endif
        return sum;
    }
};
