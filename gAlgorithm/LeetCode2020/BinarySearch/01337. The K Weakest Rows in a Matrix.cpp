// Given a m * n matrix mat of ones (representing soldiers) and zeros (representing civilians), 
// return the indexes of the k weakest rows in the matrix ordered from the weakest to the strongest.

// A row i is weaker than row j, if the number of soldiers in row i is less than the number of soldiers in row j, 
// or they have the same number of soldiers but i is less than j.
// Soldiers are always stand in the frontier of a row, that is, 
// always ones may appear first and then zeros.

// Input: mat = 
// [[1,1,0,0,0],
//  [1,1,1,1,0],
//  [1,0,0,0,0],
//  [1,1,0,0,0],
//  [1,1,1,1,1]], 
// k = 3
// Output: [2,0,3]
// Explanation: 
// The number of soldiers for each row is: 
// row 0 -> 2 
// row 1 -> 4 
// row 2 -> 1 
// row 3 -> 2 
// row 4 -> 5 
// Rows ordered from the weakest to the strongest are [2,0,3,1,4]

// Input: mat = 
// [[1,0,0,0],
//  [1,1,1,1],
//  [1,0,0,0],
//  [1,0,0,0]], 
// k = 2
// Output: [0,2]
// Explanation: 
// The number of soldiers for each row is: 
// row 0 -> 1 
// row 1 -> 4 
// row 2 -> 1 
// row 3 -> 1 
// Rows ordered from the weakest to the strongest are [0,2,3,1]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/the-k-weakest-rows-in-a-matrix
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 根据战斗力排序
// 排序后返回索引
// 
// 其实本题考察 c++::stl 的使用
// 1. accumulate 累计算子 求战力
// 2. pair 存放 战力 与 索引
// 3. sort 的自定义比较算子
// 4. 比较算子还有一个坑; 当战力相同, 则根据索引排序

class Solution {
public:
    vector<int> kWeakestRows(vector<vector<int>>& mat, int k) {
        struct {
            bool operator() (pair<int, int> &x, pair<int, int> &y) {
                return x.second == y.second ? x.first < y.first : x.second < y.second;
            }
        } mless;
        vector<pair<int, int>> power(mat.size());
        // 计算战斗力
        for (int i = 0; i < mat.size(); ++i)
            power[i] = make_pair(i, accumulate(mat[i].begin(), mat[i].end(), 0));
        // 排序战斗力
        sort(power.begin(), power.end(), mless);
        // 获取最优k
        vector<int> ans(k, 0);
        for (int i = 0; i < k; i++)
            ans[i] = power[i].first;
        return ans;
    }
};