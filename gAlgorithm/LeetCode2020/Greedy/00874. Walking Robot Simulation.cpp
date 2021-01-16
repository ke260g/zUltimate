class Solution {
public:
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        // Note: 方向步进矩阵参考 00999. Available Captures For Rook
        int d = 0; // 方向 North, East, South, West;
        static int dx[] = { 0, 1, 0, -1 }; // 方向步进矩阵
        static int dy[] = { 1, 0, -1, 0 }; // 方向步进矩阵
        vector<int> p(2, 0);
        int ans = 0;
        // 障碍物构造成集合
        set<vector<int>> mobstacles(obstacles.begin(), obstacles.end());

        for (auto c: commands) {
            if (c > 0) {
                for (int i = 1; i <= c; ++i) {
                    // 前进
                    p[0] += dx[d]; p[1] += dy[d];
                    if (mobstacles.count(p)) { // 遇到障碍物; 回去
                        p[0] -= dx[d]; p[1] -= dy[d];
                        break;
                    }
                }
                // 移动后更新结果
                ans = max(ans, p[0]*p[0] + p[1]*p[1]);
            } else if (c == -1) { // 右转
                d = (d + 1) % 4;
            } else if (c == -2) { // 左转
                d = (d + 4 - 1) % 4;
            }
        }
        return ans;
    }
};