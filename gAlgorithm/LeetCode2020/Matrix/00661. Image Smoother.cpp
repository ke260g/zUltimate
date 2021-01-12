/*
 * @lc app=leetcode id=661 lang=cpp
 *
 * [661] Image Smoother
 *
 * https://leetcode.com/problems/image-smoother/description/
 *
 * algorithms
 * Easy (52.11%)
 * Total Accepted:    52.4K
 * Total Submissions: 100.5K
 * Testcase Example:  '[[1,1,1],[1,0,1],[1,1,1]]'
 *
 * Given a 2D integer matrix M representing the gray scale of an image, you
 * need to design a smoother to make the gray scale of each cell becomes the
 * average gray scale (rounding down) of all the 8 surrounding cells and
 * itself.  If a cell has less than 8 surrounding cells, then use as many as
 * you can.
 * 
 * Example 1:
 * 
 * Input:
 * [[1,1,1],
 * ⁠[1,0,1],
 * ⁠[1,1,1]]
 * Output:
 * [[0, 0, 0],
 * ⁠[0, 0, 0],
 * ⁠[0, 0, 0]]
 * Explanation:
 * For the point (0,0), (0,2), (2,0), (2,2): floor(3/4) = floor(0.75) = 0
 * For the point (0,1), (1,0), (1,2), (2,1): floor(5/6) = floor(0.83333333) = 0
 * For the point (1,1): floor(8/9) = floor(0.88888889) = 0
 * 
 * 
 * 
 * Note:
 * 
 * The value in the given matrix is in the range of [0, 255].
 * The length and width of the given matrix are in the range of [1, 150].
 * 
 * 
 */
#include <vector>
#include <cstdio>
#include <cmath>
using namespace std;
class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& M) {
        // 自己想的原始写法; 双 85%
        // 1. 根据当前像素点; 计算 左右边界 和 上下边界
        // 2. 遍历 左右边界 和 上下边界 的矩阵
        //    求得 sum 以及 像素点个数
        // 3. 计算新图像的相同位置像素点
        vector<vector<int>> N(M.size(), vector<int>(M[0].size(), 0));
        for (int i = 0; i < M.size(); ++i) {
            for (int j = 0; j < M[0].size(); ++j) {
                // 1.
                const int pMin = i>0 ? i-1 : i;
                const int pMax = i+1 < M.size() ? i+1 : i;
                const int qMin = j>0 ? j-1 : j;
                const int qMax = j+1 < M[0].size() ? j+1 : j;

                // 2.
                int sum  = 0, count = 0;
                for (int p = pMin; p <= pMax; ++p) {
                    for (int q = qMin; q <= qMax; ++q) {
                        sum += M[p][q];
                        count++;
                    }
                }
                // 3.
                N[i][j] = floor(sum / count);
            }
        }
        return N;
    }
};
