/*
 * @lc app=leetcode id=1646 lang=cpp
 *
 * [1646] Get Maximum in Generated Array
 *
 * https://leetcode.com/problems/get-maximum-in-generated-array/description/
 *
 * algorithms
 * Easy (48.30%)
 * Total Accepted:    30K
 * Total Submissions: 56K
 * Testcase Example:  '7'
 *
 * You are given an integer n. An array nums of length n + 1 is generated in
 * the following way:

 * 
 * 
 * nums[0] = 0
 * nums[1] = 1
 * nums[2 * i] = nums[i] when 2 <= 2 * i <= n
 * nums[2 * i + 1] = nums[i] + nums[i + 1] when 2 <= 2 * i + 1 <= n
 * 
 * 
 * Return the maximum integer in the array nums​​​.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 7
 * Output: 3
 * Explanation: According to the given rules:
 * ⁠ nums[0] = 0
 * ⁠ nums[1] = 1
 * ⁠ nums[(1 * 2) = 2] = nums[1] = 1
 * ⁠ nums[(1 * 2) + 1 = 3] = nums[1] + nums[2] = 1 + 1 = 2
 * ⁠ nums[(2 * 2) = 4] = nums[2] = 1
 * ⁠ nums[(2 * 2) + 1 = 5] = nums[2] + nums[3] = 1 + 2 = 3
 * ⁠ nums[(3 * 2) = 6] = nums[3] = 2
 * ⁠ nums[(3 * 2) + 1 = 7] = nums[3] + nums[4] = 2 + 1 = 3
 * Hence, nums = [0,1,1,2,1,3,2,3], and the maximum is 3.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 2
 * Output: 1
 * Explanation: According to the given rules, the maximum between nums[0],
 * nums[1], and nums[2] is 1.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: n = 3
 * Output: 2
 * Explanation: According to the given rules, the maximum between nums[0],
 * nums[1], nums[2], and nums[3] is 2.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 0 <= n <= 100
 * 
 * 
 */
class Solution {
public:
    int getMaximumGenerated(int n) {
        // 1. 构造数组 n+1 长度
        // 2. 从 1 开始遍历
        // 3. 如果是1 那么 = 1
        //    否则 = a[i/2] 往下取整
        //    如果 i 是奇数; 额外加上 a[i/2+1]
        //    动态维护最大值
        // 返回最大值
        int answer = 0;
        vector<int> a(n+1, 0);
        for (int i = 1; i < a.size(); i++) {
            if (i != 1)
                a[i] = a[i/2] + (i&0x1)*(a[i/2 + 1]);
            else
                a[i] = 1;
            answer = max(answer, a[i]);
        }
        return answer;
    }
};
