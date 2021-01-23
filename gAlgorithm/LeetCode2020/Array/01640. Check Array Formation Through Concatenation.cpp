/*
 * @lc app=leetcode id=1640 lang=cpp
 *
 * [1640] Check Array Formation Through Concatenation
 *
 * https://leetcode.com/problems/check-array-formation-through-concatenation/description/
 *
 * algorithms
 * Easy (64.52%)
 * Total Accepted:    48.3K
 * Total Submissions: 78.2K
 * Testcase Example:  '[85]\n[[85]]'
 *
 * You are given an array of distinct integers arr and an array of integer
 * arrays pieces, where the integers in pieces are distinct. Your goal is to
 * form arr by concatenating the arrays in pieces in any order. However, you
 * are not allowed to reorder the integers in each array pieces[i].
 * 
 * Return true if it is possible to form the array arr from pieces. Otherwise,
 * return false.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: arr = [85], pieces = [[85]]
 * Output: true
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: arr = [15,88], pieces = [[88],[15]]
 * Output: true
 * Explanation: Concatenate [15] then [88]
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: arr = [49,18,16], pieces = [[16,18,49]]
 * Output: false
 * Explanation: Even though the numbers match, we cannot reorder pieces[0].
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: arr = [91,4,64,78], pieces = [[78],[4,64],[91]]
 * Output: true
 * Explanation: Concatenate [91] then [4,64] then [78]
 * 
 * Example 5:
 * 
 * 
 * Input: arr = [1,3,5,7], pieces = [[2,4,6,8]]
 * Output: false
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= pieces.length <= arr.length <= 100
 * sum(pieces[i].length) == arr.length
 * 1 <= pieces[i].length <= arr.length
 * 1 <= arr[i], pieces[i][j] <= 100
 * The integers in arr are distinct.
 * The integers in pieces are distinct (i.e., If we flatten pieces in a 1D
 * array, all the integers in this array are distinct).
 * 
 */
class Solution {
public:
    bool canFormArray(vector<int>& arr, vector<vector<int>>& pieces) {
        // 1. hash arr 的数字 和 索引
        // 2. 遍历每个piece 的每个数字
        // 3. 如果 piece 上的数字找不到; 不满足条件
        // 4. piece 非首个数字 在 arr 中出现的索引  不等于 
        //    piece 前一个数字 在 arr 中出现的索引+1; 不满足条件
        map<int, int> memo;        
        for (int i = 0; i < arr.size(); ++i)
            memo[arr[i]] = i;
        for (auto & p: pieces) {
            for (int i = 0, last = -1; i < p.size(); ++i) {
                auto curr = memo.find(p[i]);
                if (curr == memo.end()/* no num */ ||
                    (i > 0 && last+1 != curr->second))
                    return false;
                last = curr->second;
            }
        }
        return true;
    }
};
