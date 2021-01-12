/*
 * @lc app=leetcode id=605 lang=cpp
 *
 * [605] Can Place Flowers
 *
 * https://leetcode.com/problems/can-place-flowers/description/
 *
 * algorithms
 * Easy (31.90%)
 * Total Accepted:    147.4K
 * Total Submissions: 461.9K
 * Testcase Example:  '[1,0,0,0,1]\n1'
 *
 * You have a long flowerbed in which some of the plots are planted, and some
 * are not. However, flowers cannot be planted in adjacent plots.
 * 
 * Given an integer array flowerbed containing 0's and 1's, where 0 means empty
 * and 1 means not empty, and an integer n, return if n new flowers can be
 * planted in the flowerbed without violating the no-adjacent-flowers rule.
 * 
 * 
 * Example 1:
 * Input: flowerbed = [1,0,0,0,1], n = 1
 * Output: true
 * Example 2:
 * Input: flowerbed = [1,0,0,0,1], n = 2
 * Output: false
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= flowerbed.length <= 2 * 10^4
 * flowerbed[i] is 0 or 1.
 * There are no two adjacent flowers in flowerbed.
 * 0 <= n <= flowerbed.length
 * 
 * 
 */
class Solution {
public:
    int __plant(int nplot) {
        return nplot & 0x1 ? (nplot-1) / 2 : (nplot-2) / 2;
    }
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        // 1. 假设 开头和末尾 分别额外分配了一个 空花盘
        // 2. 遍历花盘
        // 3. 当遇到 已种植的花盘;
        //    说明 完整地算了一次 连续的 空花盘数量
        //    此时计算 连续空花盘数量 中 可种植数量
        //    并且重置 连续的空花盘数量
        // 4. 当遇到 空花盘; 累加连续的空花盘数量
        // 5. 最后; 要补算末尾的空花盘数量
        int plant = 0, nplot = 1/* 开头空花盘 */;
        for (auto n: flowerbed) {
            if (n == 0) {
                nplot++;
            } else {
                if (nplot)
                    plant +=__plant(nplot);
                nplot = 0;
            }
        }
        nplot++ /* 末尾空花盘 */;
        plant +=__plant(nplot);
        return plant >= n;
    }
};

// 方法2: DP + 贪心 == 跳格  (参考别人的)
// 1. 如果当前位置是1; 那么下一个可能种花的点
//    一定是 i+2;
// 2. 如果当前位置是0; 那么
// 2.1 当前位置是末尾; 那么可以种花
//     这里不需要判断左边是否为0 (后面解析)
// 2.2 当前位置不是末尾; 但右边是零; 可以种花
//     这里不需要判断左边是否为0 (后面解析)
//     下一个可能的种花点是 i+2
// 2.3 不是末尾; 且右边不是零; 不能种花
//     下一个可能的种花点是 i+3
//
// 因为我们跳格子是 2,3, 这样子步进的
// 所以除了 首个位置; 其他位置如果当前是0
// 那么都能保证其左边一定是0
class Solution {
public:
    bool canPlaceFlowers(vector<int>& flowerbed, int n) {
        int i = 0;
        while (i < flowerbed.size() && n > 0) {
            if (flowerbed[i] == 1) {
                i += 2;
            } else if (i == flowerbed.size() - 1 || flowerbed[i + 1] == 0) {
                n--;
                i += 2;
            } else {
                i += 3;
            }
        }
        return n <= 0;
    }
};