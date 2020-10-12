#include<string>
#include<vector>
#include<iostream>

using namespace std;
void permutationCore(string &s, vector<string> &n, vector<int> & visit, int idx) {
    if (idx == s.size()) {
        vector<char> ni(visit.size(), -1);
        for (int i = 0; i < visit.size(); ++i)
            ni[i] = s[visit[i]];
        string ns(ni.begin(), ni.end());
        n.push_back(ns);
        return;
    }
    for (int i = 0; i < visit.size(); ++i) {
        if (visit[i] == -1) {
            visit[i] = idx; // 记录索引
            permutationCore(s, n, visit, idx+1); // 递归下层
            visit[i] = -1;  // 清除索引
        }
    }
}

void permutation(string &s, vector<string> &n) {
    vector<int> visit(s.size(), -1);
    permutationCore(s, n, visit, 0);
}

int main( int argc, char **argv ) {
    string s = "1234";
    vector<string> n;
    permutation(s, n);
    for (int i = 0; i < n.size(); ++i) {
        cout << n[i] << endl;
    }
    return 0;
}
