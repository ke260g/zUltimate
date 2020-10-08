// Given an array of integers A sorted in non-decreasing order, 
// return an array of the squares of each number, also in sorted non-decreasing order.

// Input: [-4,-1,0,3,10]
// Output: [0,1,9,16,100]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/squares-of-a-sorted-array
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 空间复杂度: O(n)
// 时间复杂度: O(n)

class Solution {
public:
    // 1. 已排序数组存在负数;
    // 2. 最左边是负数平方最大的; 最右边是正数平方最大的
    // 3. 因此 "平方数组" 每取一个较大的数; 本质就是从原数组左边 or 右边取一个绝对值较大的
    // 4. 双指针法: i 表示左边 j表示右边
    // 5. 如果左边平方更大则选取左边的; 如果右边平方更大则取右边的
    // 6. 优化: 原数组左边平方比右边的 更大唯一的 可能就是 左边是负数且 加上右边仍然是负数
    vector<int> sortedSquares(vector<int>& A) {
        vector<int> B(A.size(), 0);
        int k = A.size()-1;
        int j = A.size()-1;
        int i = 0;
        while(i <= j) {
            // if (A[i]*A[i] < A[j]*A[j])
            if (A[i]+A[j] < 0) { // 左边数值的绝对值 大于 右边数值的绝对值
                B[k--] = A[i]*A[i];
                i++;
            } else {
                B[k--] = A[j]*A[j];
                --j;
            }            
        }
        return B;
    }
};