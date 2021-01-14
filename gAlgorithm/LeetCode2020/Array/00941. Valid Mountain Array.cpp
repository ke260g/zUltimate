/*
 * @lc app=leetcode id=941 lang=cpp
 *
 * [941] Valid Mountain Array
 *
 * https://leetcode.com/problems/valid-mountain-array/description/
 *
 * algorithms
 * Easy (33.55%)
 * Total Accepted:    128.5K
 * Total Submissions: 383.3K
 * Testcase Example:  '[2,1]'
 *
 * Given an array of integers arr, return true if and only if it is a valid
 * mountain array.
 * 
 * Recall that arr is a mountain array if and only if:
 * 
 * 
 * arr.length >= 3
 * There exists some i with 0 < i < arr.length - 1 such that:
 * 
 * arr[0] < arr[1] < ... < arr[i - 1] < arr[i] 
 * arr[i] > arr[i + 1] > ... > arr[arr.length - 1]
 * 
 * 
 * 
 * 
 * 
 * Example 1:
 * Input: arr = [2,1]
 * Output: false
 * Example 2:
 * Input: arr = [3,5,5]
 * Output: false
 * Example 3:
 * Input: arr = [0,3,2,1]
 * Output: true
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= arr.length <= 10^4
 * 0 <= arr[i] <= 10^4
 * 
 * 
 */
class Solution {
public:
    bool validMountainArray(vector<int>& arr) {
        int status = 0;
        // 状态机: 0:没上山 1:上坡 2:下坡
        //
        // 1. 平原: 干掉
        // 2. 上坡:  没上山; 状态机 0 -> 1
        //    上坡:  之前已经下坡; 干掉
        // 3. 下坡:  没上山; 干掉
        //    下坡:  之前已经上坡; 状态机 1->2
        for (int i = 0; i+1 < arr.size(); ++i) {
            if (arr[i] == arr[i+1]) // 干掉平原
                return false;
            if (arr[i] < arr[i+1]) {
                if (status == 0) // 还没上山; 首次遇到上坡
                    status = 1;
                else if (status == 2) // 已经找到 mountain 再遇到上坡
                    return false;
            } else if (arr[i] > arr[i+1]) {
                if (status == 0) // 还没上山; 首次遇到下坡
                    return false;
                else if (status == 1) // 上坡后遇到下坡
                    status = 2;
            }
        }
        return status == 2;
    }
};
