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
