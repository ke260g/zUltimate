// Given a m * n matrix grid which is sorted in non-increasing order both row-wise and column-wise. 

// Return the number of negative numbers in grid.

// Input: grid = [[4,3,2,-1],[3,2,1,-1],[1,1,-1,-2],[-1,-1,-2,-3]]
// Output: 8
// Explanation: There are 8 negatives number in the matrix.

// Input: grid = [[3,2],[1,0]]
// Output: 0

// Input: grid = [[1,-1],[-1,-1]]
// Output: 3

// Input: grid = [[-1]]
// Output: 1

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/count-negative-numbers-in-a-sorted-matrix
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 在降序数组中  二分查找法  找到第一个负数
// 通过第一个负数的索引; 求得负数的数量
// 初始值设置为 line.size() 因为刚开始没找到; 假定一整个 line 都是正数
class Solution {
public:
    int countNegatives(vector<vector<int>>& grid) {
        int count = 0;
        for (auto line: grid) {
            int l = 0, r = line.size()-1;
            int negative_cursor = line.size();
            while (l <= r) {
                int m = l + (r-l)/2;
                if (line[m] < 0) {
                    negative_cursor = m;
                    r = m - 1;
                } else {
                    l = m + 1;
                }
            }
            count += line.size() - negative_cursor;
        }
        return count;
    }
};