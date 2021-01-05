// Given a non-negative integer numRows, 
// generate the first numRows of Pascal's triangle.

// Input: 5
// Output:
// [
//      [1],
//     [1,1],
//    [1,2,1],
//   [1,3,3,1],
//  [1,4,6,4,1]
// ]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/pascals-triangle
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> ans;
        for (int i = 0; i < numRows; ++i) {
            int size = i + 1;
            vector<int> row(size, 0);
            row[0] = 1;              // 每行首数值是1
            row[row.size()-1] = 1;   // 每行末数值是1
            if (i > 0) {
                for (int j = 1; j < row.size()-1; j++) {
                    row[j] = ans[i-1][j] + ans[i-1][j-1];
                }
            }
            ans.push_back(row);
        }
        return ans;
    }
};