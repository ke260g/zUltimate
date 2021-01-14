/*
 * @lc app=leetcode id=914 lang=cpp
 *
 * [914] X of a Kind in a Deck of Cards
 *
 * https://leetcode.com/problems/x-of-a-kind-in-a-deck-of-cards/description/
 *
 * algorithms
 * Easy (34.40%)
 * Total Accepted:    57.9K
 * Total Submissions: 168.3K
 * Testcase Example:  '[1,2,3,4,4,3,2,1]'
 *
 * In a deck of cards, each card has an integer written on it.
 * 
 * Return true if and only if you can choose X >= 2 such that it is possible to
 * split the entire deck into 1 or more groups of cards, where:
 * 
 * 
 * Each group has exactly X cards.
 * All the cards in each group have the same integer.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: deck = [1,2,3,4,4,3,2,1]
 * Output: true
 * Explanation: Possible partition [1,1],[2,2],[3,3],[4,4].
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: deck = [1,1,1,2,2,2,3,3]
 * Output: false´
 * Explanation: No possible partition.
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: deck = [1]
 * Output: false
 * Explanation: No possible partition.
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: deck = [1,1]
 * Output: true
 * Explanation: Possible partition [1,1].
 * 
 * 
 * Example 5:
 * 
 * 
 * Input: deck = [1,1,2,2,2,2]
 * Output: true
 * Explanation: Possible partition [1,1],[2,2],[2,2].
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= deck.length <= 10^4
 * 0 <= deck[i] < 10^4
 * 
 * 
 */
#include <climits>
// #include "prettyprint.hpp"
class Solution {
public:
    bool hasGroupsSizeX(vector<int>& deck) {
        // 题意: deck 所有数值的频率 的 是否存在 >= 2 的公因数
        //
        // 1. map 求频率
        // 2. set 使频率unique (这里需要排序的)
        // 3. 从 [ 2, set[0](最小的频率) ] 区间中 逐个遍历
        //    选取为 候选的公因数
        // 4. 然后遍历 set 每个频率(包括第0个);
        //    判断是否能 整除 上步骤中的 候选公因数
        // 5. 如果找到满足条件的公因数; 那就返回 true
        map<int, int> card;
        for (const auto c: deck)
           card[c]++; 

        set<int> mset;
        for (const auto p: card)
            mset.insert(p.second);
        
        for (int i = 2; i <= *(mset.begin()); i++) {
            if (all_of(mset.begin(), mset.end(),
                [&](int x) { return !(x % i);}))
                return true;
        }

        return false;
    }
};
