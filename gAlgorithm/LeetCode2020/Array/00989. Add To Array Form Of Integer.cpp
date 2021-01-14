/*
 * @lc app=leetcode id=989 lang=cpp
 *
 * [989] Add to Array-Form of Integer
 *
 * https://leetcode.com/problems/add-to-array-form-of-integer/description/
 *
 * algorithms
 * Easy (44.66%)
 * Total Accepted:    58.8K
 * Total Submissions: 131.6K
 * Testcase Example:  '[1,2,0,0]\n34'
 *
 * For a non-negative integer X, the array-form of X is an array of its digits
 * in left to right order.  For example, if X = 1231, then the array form is
 * [1,2,3,1].
 * 
 * Given the array-form A of a non-negative integer X, return the array-form of
 * the integer X+K.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: A = [1,2,0,0], K = 34
 * Output: [1,2,3,4]
 * Explanation: 1200 + 34 = 1234
 * 
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: A = [2,7,4], K = 181
 * Output: [4,5,5]
 * Explanation: 274 + 181 = 455
 * 
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: A = [2,1,5], K = 806
 * Output: [1,0,2,1]
 * Explanation: 215 + 806 = 1021
 * 
 * 
 * 
 * Example 4:
 * 
 * 
 * Input: A = [9,9,9,9,9,9,9,9,9,9], K = 1
 * Output: [1,0,0,0,0,0,0,0,0,0,0]
 * Explanation: 9999999999 + 1 = 10000000000
 * 
 * 
 * 
 * 
 * Note：
 * 
 * 
 * 1 <= A.length <= 10000
 * 0 <= A[i] <= 9
 * 0 <= K <= 10000
 * If A.length > 1, then A[0] != 0
 * 
 * 
 * 
 * 
 * 
 */
class Solution {
public:
    vector<int> __addVectors(vector<int> &x, vector<int> &y) {
        // 00415. Add Strings.cpp
        auto addone = [&](int &l, int r, int &give) {
            l += r + give; 
            give = l / 10;    
            l %= 10;
        };
        int i = 0, give = 0;
        if (x.size() < y.size())
            swap(x, y);
        for (; i < y.size(); i++)
            addone(x[i], y[i], give);
        for (; i < x.size() && give; i++)
            addone(x[i], 0, give);
        if (give)
            x.push_back(1);
        return x;
    }

    vector<int> addToArrayForm(vector<int>& A, int K) {
        // 1. 把A 翻转
        // 2. K 转化为逆序的 vector
        // 3. 使用 00415. Add Strings.cpp 的 vector 加法
        // 4. 再翻转
        reverse(A.begin(), A.end());
        vector<int> Y, Z;
        while (K) {
            Y.push_back(K % 10);
            K /= 10;
        }
        Z = __addVectors(A, Y);
        reverse(Z.begin(), Z.end());
        return Z;
    }
};
