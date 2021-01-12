/*
 * @lc app=leetcode id=888 lang=cpp
 *
 * [888] Fair Candy Swap
 *
 * https://leetcode.com/problems/fair-candy-swap/description/
 *
 * algorithms
 * Easy (58.72%)
 * Total Accepted:    57K
 * Total Submissions: 97.1K
 * Testcase Example:  '[1,1]\n[2,2]'
 *
 * Alice and Bob have candy bars of different sizes: A[i] is the size of the
 * i-th bar of candy that Alice has, and B[j] is the size of the j-th bar of
 * candy that Bob has.
 * 
 * Since they are friends, they would like to exchange one candy bar each so
 * that after the exchange, they both have the same total amount of candy.
 * (The total amount of candy a person has is the sum of the sizes of candy
 * bars they have.)
 * 
 * Return an integer array ans where ans[0] is the size of the candy bar that
 * Alice must exchange, and ans[1] is the size of the candy bar that Bob must
 * exchange.
 * 
 * If there are multiple answers, you may return any one of them.  It is
 * guaranteed an answer exists.
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: A = [1,1], B = [2,2]
 * Output: [1,2]
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: A = [1,2], B = [2,3]
 * Output: [1,2]
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: A = [2], B = [1,3]
 * Output: [2,3]
 * 
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: A = [1,2,5], B = [2,4]
 * Output: [5,4]
 * 
 * 
 * 
 * 
 * Note:
 * 
 * 
 * 1 <= A.length <= 10000
 * 1 <= B.length <= 10000
 * 1 <= A[i] <= 100000
 * 1 <= B[i] <= 100000
 * It is guaranteed that Alice and Bob have different total amounts of
 * candy.
 * It is guaranteed there exists an answer.
 * 
 * 
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    /**
     * @brief 自己想的 (几乎就是题解)
     * 1. 求 A 的总数  和 求 B 的总数
     * 2. 构造集合 B
     * 3. 实际上待交换的糖果满足一个条件
     *    A[x] - B[y] = (sum(A) - sum(B)) / 2
     *    目的就是找到 A[x] B[y]
     * 4. 遍历 A; 针对每个A[x] 求理想的 B[y] 是否在 集合B 中存在
     *    存在就返回答案
     */
    vector<int> fairCandySwap(vector<int>& A, vector<int>& B) {
        vector<int> ans;
        int suma = 0, sumb = 0;
        set<int> setb;
        for (auto n: A) suma += n;
        for (auto n: B) {
            setb.insert(n);
            sumb += n;
        }
        int diff = (suma - sumb) / 2;
        for (int i = 0; i < A.size(); ++i) {
            if (setb.count(A[i] - diff)) {
                ans.push_back(A[i]);
                ans.push_back(A[i] - diff);
                break;
            }
        }
        return ans;
    }
};