/*
 * @lc app=leetcode id=896 lang=cpp
 *
 * [896] Monotonic Array
 *
 * https://leetcode.com/problems/monotonic-array/description/
 *
 * algorithms
 * Easy (58.01%)
 * Total Accepted:    135.7K
 * Total Submissions: 233.9K
 * Testcase Example:  '[1,2,2,3]'
 *
 * An array is monotonic if it is either monotone increasing or monotone
 * decreasing.
 * 
 * An array A is monotone increasing if for all i <= j, A[i] <= A[j].  An array
 * A is monotone decreasing if for all i <= j, A[i] >= A[j].
 * 
 * Return true if and only if the given array A is monotonic.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: [1,2,2,3]
 * Output: true
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: [6,5,4,4]
 * Output: true
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: [1,3,2]
 * Output: false
 * 
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: [1,2,4,5]
 * Output: true
 * 
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: [1,1,1]
 * Output: true
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * 1 <= A.length <= 50000
 * -100000 <= A[i] <= 100000
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    bool isMonotonic(vector<int>& A) {
        int status = 0;
        // 一次遍历; 动态记录ta的上升 or 下降趋势
        // 注意: 当趋势没有变化; 不要更新状态变量
        //      b) 不要直接使用 status*ns 判断趋势不同; 因为会溢出
        for (int i = 0; i+1 < A.size(); i++) {
            int ns = A[i] - A[i+1];
            if (ns) {
                ns = ns > 0 ? 1 : -1;
                if (status && ns != status)
                    return false;
                status = ns;
            }
        }
        return true; 
    }
};