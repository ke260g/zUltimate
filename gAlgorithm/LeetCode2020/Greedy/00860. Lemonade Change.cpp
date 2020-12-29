// 柠檬水找零

// 在柠檬水摊上，每一杯柠檬水的售价为 5 美元。

// 顾客排队购买你的产品，（按账单 bills 支付的顺序）一次购买一杯。

// 每位顾客只买一杯柠檬水，然后向你付 5 美元、10 美元或 20 美元。
// 你必须给每个顾客正确找零，
// 也就是说净交易是每位顾客向你支付 5 美元。

// 注意，一开始你手头没有任何零钱。
// 如果你能给每位顾客正确找零，返回 true ，否则返回 false 。

// 输入：[5,5,5,10,20]
// 输出：true
// 解释：
// 前 3 位顾客那里，我们按顺序收取 3 张 5 美元的钞票。
// 第 4 位顾客那里，我们收取一张 10 美元的钞票，并返还 5 美元。
// 第 5 位顾客那里，我们找还一张 10 美元的钞票和一张 5 美元的钞票。
// 由于所有客户都得到了正确的找零，所以我们输出 true。

// 输入：[5,5,10,10,20]
// 输出：false
// 解释：
// 前 2 位顾客那里，我们按顺序收取 2 张 5 美元的钞票。
// 对于接下来的 2 位顾客，我们收取一张 10 美元的钞票，然后返还 5 美元。
// 对于最后一位顾客，我们无法退回 15 美元，因为我们现在只有两张 10 美元的钞票。
// 由于不是每位顾客都得到了正确的找零，所以答案是 false。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/lemonade-change
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 记录 5 块 和 10 块 的张数
// case 顾客给的钱
//     5 块： 不找零
//    10 块:  找5块
//    15 块:  找3张5块 or 1张5块+1张10块
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        // packet[0]: count for 5
        // packet[1]: count for 10
        vector<int> packet(2, 0);
        for (auto bill: bills) {
            switch (bill) {
                case 5:
                    packet[0]++;
                    break;
                case 10:
                    if (packet[0] > 0) { // 找零 1张5块
                        packet[0]--;
                    } else
                        return false;
                    packet[1]++;
                    break;
                case 20:
                    if (packet[0] > 0 && packet[1] > 0) {// 找零 1张10块 和 1张5块
                        packet[1]--;
                        packet[0]--;
                    } else if (packet[0] > 2) { // 找3张5块
                        packet[0] -= 3;
                    } else
                        return false;
                    break;
                default:
                    return false;
            }
        }
        return true;
    }
};