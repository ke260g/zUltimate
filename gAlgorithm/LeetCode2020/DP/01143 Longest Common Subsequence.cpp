// 主要动态规划矩阵; 
// 特殊情况: 往前扩展一行和一列; 以及遍历都是从1开始的.

// 经典算法问题; LCS 不必多说
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int x = text1.size();
        int y = text2.size();
        vector<vector<int>> mat(x+1, vector<int>(y+1, 0));
        for (int i = 1; i < x+1; ++i) {
            for (int j = 1; j < y+1; ++j) {
                if (text1[i-1] == text2[j-1])
                    mat[i][j] = mat[i-1][j-1] + 1;
                else
                    mat[i][j] = max(mat[i][j-1], mat[i-1][j]);
            }
        }
        return mat[x][y];
    }
};