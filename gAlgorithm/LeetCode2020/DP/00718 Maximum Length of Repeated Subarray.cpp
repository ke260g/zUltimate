// Given two integer arrays A and B, 
// return the maximum length of an subarray that appears in both arrays.

// Input:
// A: [1,2,3,2,1]
// B: [3,2,1,4,7]
// Output: 3
// Explanation: 
// The repeated subarray with maximum length is [3, 2, 1].

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/maximum-length-of-repeated-subarray
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 题目要求算得是 公共子数组; !!!即连续的!!!
// 求最大 连续公共 子数列

// 所以在 LCS 基础算法中 `A[i-1] != B[j-1]` 时 `matrix[i][j] = 0`
// 且最后不能 `return matrix[A.size()][B.size()]`
// 需要遍历时 动态更新结果

class Solution {
public:
    int findLength(vector<int>& A, vector<int>& B) {
        int x = A.size();
        int y = B.size();
        int result = 0;
        vector<vector<int>> matrix(x+1, vector<int>(y+1, 0));
        for (int i = 1; i < x+1; ++i) {
            for (int j = 1; j < y+1; ++j) {
                if (A[i-1] == B[j-1]) {
                    matrix[i][j] = matrix[i-1][j-1] + 1;
                    result = max(result, matrix[i][j]);
                }
            }
        } 
        return result;
    }
};