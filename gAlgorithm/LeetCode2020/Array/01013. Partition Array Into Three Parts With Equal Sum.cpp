/*
 * @lc app=leetcode id=1013 lang=cpp
 *
 * [1013] Partition Array Into Three Parts With Equal Sum
 *
 * https://leetcode.com/problems/partition-array-into-three-parts-with-equal-sum/description/
 *
 * algorithms
 * Easy (49.53%)
 * Total Accepted:    42.4K
 * Total Submissions: 85.7K
 * Testcase Example:  '[0,2,1,-6,6,-7,9,1,2,0,1]'
 *
 * Given an array of integers arr, return true if we can partition the array
 * into three non-empty parts with equal sums.
 * 
 * Formally, we can partition the array if we can find indexes i + 1 < j with
 * (arr[0] + arr[1] + ... + arr[i] == arr[i + 1] + arr[i + 2] + ... + arr[j -
 * 1] == arr[j] + arr[j + 1] + ... + arr[arr.length - 1])
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [0,2,1,-6,6,-7,9,1,2,0,1]
 * Output: true
 * Explanation: 0 + 2 + 1 = -6 + 6 - 7 + 9 + 1 = 2 + 0 + 1
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [0,2,1,-6,6,7,9,-1,2,0,1]
 * Output: false
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: arr = [3,3,6,5,-2,2,5,1,-9,4]
 * Output: true
 * Explanation: 3 + 3 = 6 = 5 - 2 + 2 + 5 + 1 - 9 + 4
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 3 <= arr.length <= 5 * 10^4
 * -10^4 <= arr[i] <= 10^4
 * 
 * 
 */
class Solution {
public:
    bool canThreePartsEqualSum(vector<int>& arr) {
        int sum = 0;
        // 1. 算出总和, 拆分值
        // 2. 判断总和 是否 3的整除
        // 3. 遍历 0~arr.size()-1  (这里必须留下最右边一个成员)
        // 4. 状态机法: 累加已经遍历的值
        //    状态0: 还没找到左边的分界; 如果 累加值 == 拆分值
        //           那么状态切换
        //    状态1: 已经找到左边的分界; 如果 累加值 - 拆分值 == sum - 累加值
        //           那么满足条件;
        //
        // Note: 参考 00724. Find Pivot Index; 
        //       用 "总和" 以及 "前缀和" 简化掉 "前缀和数组"
        for (const auto n : arr) sum += n;        
        if (sum % 3) return false;
        int status = 0, curr = 0;

        for (int i = 0; i < arr.size()-1/* 最右边必须留有一个 */; ++i) {
            curr += arr[i];
            if (!status)
                status = (curr == sum / 3);
            else if (curr - sum / 3 == sum - curr)
                return true;
        }
        return false;
    }
};