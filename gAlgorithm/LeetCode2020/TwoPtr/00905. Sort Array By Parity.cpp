/*
 * @lc app=leetcode id=905 lang=cpp
 *
 * [905] Sort Array By Parity
 *
 * https://leetcode.com/problems/sort-array-by-parity/description/
 *
 * algorithms
 * Easy (74.92%)
 * Total Accepted:    286K
 * Total Submissions: 381.7K
 * Testcase Example:  '[3,1,2,4]'
 *
 * Given an array A of non-negative integers, return an array consisting of all
 * the even elements of A, followed by all the odd elements of A.
 * 
 * You may return any answer array that satisfies this condition.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: [3,1,2,4]
 * Output: [2,4,3,1]
 * The outputs [4,2,3,1], [2,4,1,3], and [4,2,1,3] would also be accepted.
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * 1 <= A.length <= 5000
 * 0 <= A[i] <= 5000
 * 
 * 
 * 
 */

class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& A) {
         // 经典双指针题 - 窗口法
        // [0, odd) even number
        // [odd, i) odd  number
        // [i, end) unknown
        int odd = 0;
        for (int i = 0; i < A.size(); i++) {
            if (!(A[i] & 0x1))
                swap(A[i], A[odd++]);
        }
        return A;
    }
};