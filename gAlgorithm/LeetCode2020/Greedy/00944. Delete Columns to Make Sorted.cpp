// We are given an array A of N lowercase letter strings, all of the same length.

// Now, we may choose any set of deletion indices, and for each string,
// we delete all the characters in those indices.

// For example, if we have an array A = ["abcdef","uvwxyz"] and deletion indices {0, 2, 3}, 
// then the final array after deletions is ["bef", "vyz"],
// and the remaining columns of A are ["b","v"], ["e","y"], and ["f","z"].
//  (Formally, the c-th column is [A[0][c], A[1][c], ..., A[A.length-1][c]]).

// Suppose we chose a set of deletion indices D such that after deletions, 
// each remaining column in A is in non-decreasing sorted order.

// Return the minimum possible value of D.length.

// Input: A = ["cba","daf","ghi"]
// Output: 1
// Explanation: 
// After choosing D = {1}, each column ["c","d","g"] and ["a","f","i"] are in non-decreasing sorted order.
// If we chose D = {}, then a column ["b","a","h"] would not be in non-decreasing sorted order.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/delete-columns-to-make-sorted
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 每行每列进行递增检测; 一旦发现该列不符合要求; 即把该列删除
// 计数++
class Solution {
public:
    int minDeletionSize(vector<string>& A) {
        int record = 0;
        for (int i = 0; i < A[0].size(); ++i) {
            for (int j = 1; j < A.size(); j++) {
                if (A[j][i] < A[j-1][i]) {
                    record++;
                    break; // 一旦发现即不再遍历该列
                }
            }
        }
        return record;
    }
};