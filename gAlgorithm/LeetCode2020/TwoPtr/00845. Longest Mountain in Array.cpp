// Let's call any (contiguous) subarray B (of A) a mountain if the following properties hold:
//     B.length >= 3
//     There exists some 0 < i < B.length - 1 such that 
//     B[0] < B[1] < ... B[i-1] < B[i] > B[i+1] > ... > B[B.length - 1]

// (Note that B could be any subarray of A, including the entire array A.)
// Given an array A of integers, return the length of the longest mountain. 
// Return 0 if there is no mountain.

// Input: [2,1,4,7,3,2,5]
// Output: 5
// Explanation: The largest mountain is [1,4,7,3,2] which has length 5.

// Input: [2,2,2]
// Output: 0
// Explanation: There is no mountain.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/longest-mountain-in-array
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 缓存之前的山势; 根据山势找到 谷底-谷峰-谷底; 从而计算山脉
// 这里好几个坑
// 1. 存在平原; 因此 当前山势可能为0; 
//    上升到平原也是谷峰
//    下降到平原也是谷底
// 2. 第一个元素之前假设是平原
// 3. 最后一个元素如果是谷底; 别忘了计算山脉
// 4. 干掉平原
//    [ 1,2,2,1 ] 不是山脉
//    [ 2,1,1,2 ] 不是山脉
//    [ 1,1,2,1,1,1] 其中 [1,2,1] 是山脉
class Solution {
public:
    int longestMountain(vector<int>& A) {
        int len = A.size();
        int memo[2] = { -1, -1 }; // [0] 前一个谷底; [1] 前一个谷峰
        int longest = 0;
        int direction = 0;
        int pd = 0;
        for (int i = 1; i < A.size(); i++) {
            // 判断山势的走向
            if (A[i] == A[i-1])
                direction = 0;  // 表示平原
            else if (A[i] < A[i-1])
                direction = -1; // 表示下坡
            else if (A[i] > A[i-1])
                direction = 1;  // 表示上坡

            if (pd < direction) {
                // 发现谷底
                if (memo[0] != -1 && memo[1] != -1) {
                    // 形成了一个山脉
                    longest = max(longest, (i-1) - memo[0] + 1);
                    memo[1] = -1; // 置空谷峰
                }
                memo[0] = i - 1;
            } else if (pd > direction) {
                // 发现谷峰
                if (memo[0] != -1)
                    memo[1] = i-1;
            }

            if (!direction) memo[0] = -1; // 干掉平原
            pd = direction;
        }
        // 最后一个数值处于下降趋势
        if (pd < 0 && memo[1] != -1 && memo[0] != -1) 
            longest = max(longest, 1 + (len-1) - memo[0]);
        return longest;
    }
};