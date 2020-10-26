// Given numBottles full water bottles, you can exchange numExchange empty water bottles for one full water bottle.

// The operation of drinking a full water bottle turns it into an empty bottle.

// Return the maximum number of water bottles you can drink.

// Input: numBottles = 9, numExchange = 3
// Output: 13
// Explanation: You can exchange 3 empty bottles to get 1 full water bottle.
// Number of water bottles you can drink: 9 + 3 + 1 = 13.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/water-bottles
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 喝水换瓶子问题
// 给定初始瓶子数 以及兑换比例
// 求最终能喝的瓶子数
class Solution {
public:
    int numWaterBottles(int numBottles, int numExchange) {
        int drink_count = 0;
        int remain = 0;
        // 剩余瓶子可以用来兑换; 注意是大于等于
        while (numBottles >= numExchange) {
            // 不够用于兑换的；先不喝
            remain = numBottles % numExchange;
            numBottles -= remain;
            // 喝掉
            drink_count += numBottles;
            // 喝掉后换回来的
            numBottles /= numExchange;
            // 再算上不够用于兑换的
            numBottles += remain;
        }

        // 最后喝下不能用于兑换的瓶子
        drink_count += numBottles;
        numBottles -= numBottles;
        return drink_count;
    }
};