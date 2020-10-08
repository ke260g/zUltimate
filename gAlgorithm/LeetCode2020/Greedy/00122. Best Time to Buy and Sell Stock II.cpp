// 对比 00121 相似问题
// 给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。
// 设计一个算法来计算你所能获取的最大利润。你可以尽可能地完成更多的交易（多次买卖一支股票）。
// 注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。
//
// 输入: [7,1,5,3,6,4]
// 输出: 7
// 解释: 在第 2 天（股票价格 = 1）的时候买入，在第 3 天（股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。
//      随后，在第 4 天（股票价格 = 3）的时候买入，在第 5 天（股票价格 = 6）的时候卖出, 这笔交易所能获得利润 = 6-3 = 3 。
//
// 输入: [1,2,3,4,5]
// 输出: 4
// 解释: 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。
//      注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。
//      因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。
//
// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-ii
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) return 0;
        int buyin = prices[0];
        int profit = 0;
        int curr_profit = 0;
        // 1.1 当今天地股价跌了;
        // 1.2 那就在昨天卖了; curr_profit 是假设当天卖掉了的利润
        // 1.3 今天买入
        // 1.4 curr_profit = 0; // 因为今天买入; 如果卖掉则没赚
        // 2.  今天股价升了; 那就假设卖掉; 更新得到利润
        // 3.  最后必须卖掉
        for (int i = 1; i < prices.size(); ++i) {
            if (prices[i] < prices[i-1]) {
                profit += curr_profit;
                buyin = prices[i];
                curr_profit = 0;
            } else
                curr_profit = prices[i] - buyin;
        }
        profit += curr_profit;
        return profit;
    }
};