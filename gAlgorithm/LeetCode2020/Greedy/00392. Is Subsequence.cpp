class Solution {
public:
    bool isSubsequence(string s, string t) {
        // 用dp做复杂度是 O(s.size() * t.size)
        // 最长不连续公共子序列 长度是否等于s
        // vector<vector<int>> dp(t.size()+1, vector<int>(s.size()+1, 0));
        // for (int i = 1; i <= t.size(); ++i) {
        //     for (int j = 1; j <= s.size(); ++j) {
        //         if (t[i-1] == s[j-1])
        //             dp[i][j] = dp[i-1][j-1] + 1;
        //         else
        //             dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
        //     }
        // }
        // return dp[t.size()][s.size()] == s.size();

        // 双指针贪心算法 O(s.size() + t.size())
        int i = 0, j = 0;
        while(i < s.size() && j < t.size()) {
            if (s[i] == t[j]) {
                ++i;
            }
            ++j;
        }
        return s.size() == i /* 遍历到末尾 */;
    }
};