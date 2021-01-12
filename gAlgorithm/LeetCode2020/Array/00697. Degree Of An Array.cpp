/*
 * @lc app=leetcode id=697 lang=cpp
 *
 * [697] Degree of an Array
 *
 * https://leetcode.com/problems/degree-of-an-array/description/
 *
 * algorithms
 * Easy (54.29%)
 * Total Accepted:    98.2K
 * Total Submissions: 180.9K
 * Testcase Example:  '[1,2,2,3,1]'
 *
 * Given a non-empty array of non-negative integers nums, the degree of this
 * array is defined as the maximum frequency of any one of its elements.
 * 
 * Your task is to find the smallest possible length of a (contiguous) subarray
 * of nums, that has the same degree as nums.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: nums = [1,2,2,3,1]
 * Output: 2
 * Explanation: 
 * The input array has a degree of 2 because both elements 1 and 2 appear
 * twice.
 * Of the subarrays that have the same degree:
 * [1, 2, 2, 3, 1], [1, 2, 2, 3], [2, 2, 3, 1], [1, 2, 2], [2, 2, 3], [2, 2]
 * The shortest length is 2. So return 2.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: nums = [1,2,2,3,1,4,2]
 * Output: 6
 * Explanation: 
 * The degree is 3 because the element 2 is repeated 3 times.
 * So [2,2,3,1,4,2] is the shortest subarray, therefore returning 6.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * nums.length will be between 1 and 50,000.
 * nums[i] will be an integer between 0 and 49,999.
 * 
 * 
 */
#include <map>
#include <vector>
using namespace std;

class Solution {
    public:
        int findShortestSubArray(vector<int>& nums) {
            map<int, pair<int, int>> memo; // value : < count, lastIndex >
            int degree = 0, length = 0;
            for (int i = 0; i < nums.size(); ++i) {
                if (!memo[nums[i]].first)
                    memo[nums[i]].second = i;
                memo[nums[i]].first++;

                int d = memo[nums[i]].first;

                if (d == degree)
                    length = min(length, i - memo[nums[i]].second + 1);
                else if (d > degree) {
                    degree = d;
                    length = i - memo[nums[i]].second + 1;
                }
            }
            return length;
        }
};
