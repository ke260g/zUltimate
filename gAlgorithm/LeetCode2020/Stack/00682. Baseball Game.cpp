// 你现在是棒球比赛记录员。
// 给定一个字符串列表，每个字符串可以是以下四种类型之一：
// 1.整数（一轮的得分）：直接表示您在本轮中获得的积分数。
// 2. "+"（一轮的得分）：表示本轮获得的得分是前两轮有效 回合得分的总和。
// 3. "D"（一轮的得分）：表示本轮获得的得分是前一轮有效 回合得分的两倍。
// 4. "C"（一个操作，这不是一个回合的分数）：表示您获得的最后一个有效 回合的分数是无效的，应该被移除。

// 每一轮的操作都是永久性的，可能会对前一轮和后一轮产生影响。
// 你需要返回你在所有回合中得分的总和。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/baseball-game
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 棒球比赛; 按照题目要求即可
class Solution {
public:
    int calPoints(vector<string>& ops) {
        stack<int> point;
        for (int i = 0; i < ops.size(); ++i) {
            if (ops[i] == "+") {
                int p = point.top();
                point.pop();
                int pp = point.top();
                point.push(p);
                point.push(p+pp);
            } else if (ops[i] == "D") {
                point.push(point.top()*2);
            } else if (ops[i] == "C") {
                point.pop();
            } else {
                point.push(stoi(ops[i]));
            }
        }
        int final_point = 0;
        while (!point.empty()) {
            final_point += point.top();
            point.pop();
        }
        return final_point;
    }
};