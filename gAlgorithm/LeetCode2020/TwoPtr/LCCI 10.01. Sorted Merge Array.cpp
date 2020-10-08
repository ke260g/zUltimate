// You are given two sorted arrays, A and B, where A has a large enough buffer at the end to hold B. 
// Write a method to merge B into A in sorted order.

// Initially the number of elements in A and B are m and n respectively.

// Input:
// A = [1,2,3,0,0,0], m = 3
// B = [2,5,6],       n = 3

// Output: [1,2,2,3,5,6]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/sorted-merge-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 自己写的; 双80%
// 1. 分别从A, B 的 m, n 往前遍历;
// 2. 把较大的 A[i] or B[j] 放到A的末尾
// 3. 把B[0:n) 剩下的; 继续放到前面A的前面 (实际上A[0:m) 如果有剩下, 那么一定是排序好的
class Solution {
public:
    void merge(vector<int>& A, int m, vector<int>& B, int n) {
        int k = A.size()-1;
        int i = m-1, j=n-1;
        while (i >= 0 && j >= 0)  {
            if (A[i] > B[j])
                A[k--] = A[i--];
            else
                A[k--] = B[j--];
        }
        while (j >= 0)
            A[k--] = B[j--];
    }
};