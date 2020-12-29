// 数轴上放置了一些筹码，每个筹码的位置存在数组 chips 当中。

// 你可以对 任何筹码 执行下面两种操作之一（不限操作次数，0 次也可以）：

//     将第 i 个筹码向左或者右移动 2 个单位，代价为 0。
//     将第 i 个筹码向左或者右移动 1 个单位，代价为 1。

// 最开始的时候，同一位置上也可能放着两个或者更多的筹码。
// 返回将所有筹码移动到同一位置（任意位置）上所需要的最小代价。

// chips:    o o o
// position: 1 2 3
// 输入：chips = [1,2,3]
// 输出：1
// 解释：第二个筹码移动到位置三的代价是 1，
//       第一个筹码移动到位置三的代价是 0，总代价为 1。

//             o
//             o o
// chips:      o o
// position: 1 2 3
// 输入：chips = [2,2,2,3,3]
// 输出：2
// 解释：第四和第五个筹码移动到位置二的代价都是 1，所以最小总代价为 2。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/minimum-cost-to-move-chips-to-the-same-position
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 分析:
//   1. 相隔一列的 chips 可以随意移动; 不消耗;
//   2. 所以可以无消耗地把 chips 构造成两列
//   3. 且最后紧邻的两列只能 有消耗地地移动
//   4. 这个时候就把 矮的列移到高地列
class Solution {
public:
    int minCostToMoveChips(vector<int>& position) {
        // 问题的本质是求:
        // min(计数索引的成员和, 偶数索引的成员和)
        int sum_of_odd_index = 0, sum_of_even_index = 0;
        for (int i = 0; i < position.size(); ++i) {
            if (position[i] & 0x1)
                sum_of_odd_index++;
            else
                sum_of_even_index++;
        }
        return min(sum_of_odd_index, sum_of_even_index);
    }
};