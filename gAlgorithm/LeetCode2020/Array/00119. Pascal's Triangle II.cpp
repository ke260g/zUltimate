// Given an integer rowIndex, 
// return the rowIndex-th row of the Pascal's triangle.

// Notice that the row index starts from 0.

// Input: rowIndex = 3
// Output: [1,3,3,1]

// Input: rowIndex = 0
// Output: [1]

// Input: rowIndex = 1
// Output: [1,1]

// 两个vector;
// 一个记录上一行; 一个记录当前行
// 每次循环开始; 把上一行clear; 然后把当前行 交换为上一行
// 每次循环结束; 当前行就是待返回的答案
class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> prev, curr;
        for (int i = 0; i < rowIndex+1; ++i) {
            prev.clear();
            swap(prev, curr);

            int size = i + 1;
            curr.assign(size, 0);
            curr[0] = 1;             // 每行首数值是1
            curr[curr.size()-1] = 1; // 每行末数值是1
            if (i > 0) {
                for (int j = 1; j < curr.size()-1; j++) {
                    curr[j] = prev[j] + prev[j-1];
                }
            }
        }
        return curr;
    }
};