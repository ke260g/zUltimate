class Solution {
public:
    enum {
        north = 0,
        east,
        south,
        west
    };
    int robotSim(vector<int>& commands, vector<vector<int>>& obstacles) {
        set<pair<int, int>> obstaclesSet;
        for (auto obstacle: obstacles)
            obstaclesSet.insert(make_pair(obstacle[0], obstacle[1]));
        
        int d = north;
        int ans = 0;
        int x = 0, y = 0;

        // 根据指令; 更新 (x, y) 坐标
        // -2, -1 更新方向 (方向有4个)
        // 1 <= c <= 9 移动坐标
        for (auto command: commands) {
            if (command == -2) {
                // 左转
                d = (d + 4 - 1) % 4; // 环状数组的 左移
            } else if (command == -1) {
                // 右转
                d = (d + 1) % 4;     // 环状数组的 右移
            } else if (1 <= command && command <= 9) {
                // 逐步往前试探
                for (int i = 1; i <= command; ++i) {
                    int _x = x, _y = y;
                    // 根据方向进行移动
                    if (d == north)
                        _y++;
                    else if (d == east)
                        _x++;
                    else if (d == west)
                        _x--;
                    else if (d == south)
                        _y--;
                    // 试探; 有没有碰到障碍物
                    if (obstaclesSet.find(make_pair(_x, _y)) == obstaclesSet.end()) {
                        // 更新坐标
                        x = _x;
                        y = _y;
                        // 更新结果
                        ans = max(ans, x*x+y*y);
                    } else {
                        // 碰到障碍物了 走不下去
                        break;
                    }
                }
            }
        }
        return ans;
    }
};