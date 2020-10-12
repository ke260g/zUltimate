// 给定序列; 枚举序列的所有子序列
// 2进制思想; 每个成员要么出现要么不出现;
// 递归实现; (形同8皇后问题)
#include <vector>
#include <string>
#include <iostream>
using namespace std;

void subsetCore(const vector<int> & seq, vector<vector<int>> & res, vector<int> & tmp, int idx) {
        if (idx == seq.size()) {
                res.push_back(tmp);
                return;
        }
        tmp.push_back(seq[idx]);
        subsetCore(seq, res, tmp, idx+1);
        tmp.pop_back();
        subsetCore(seq, res, tmp, idx+1);
}

void subset(const vector<int> & seq, vector<vector<int>> & res) {
        vector<int> tmp;
        subsetCore(seq, res, tmp, 0);
}

int main(void) {
        int a[] = { 1, 2, 3, 4, 5 };
        vector<int> seq(a, a + 5);
        vector<vector<int>> res;
        subset(seq, res);
        for (int i = 0; i < res.size(); ++i) {
                cout << "[ ";
                for (int j = 0; j < res[i].size(); ++j) {
                        cout << res[i][j] << " ";
                }
                cout << "]" << endl;

        }
        return 0;
}