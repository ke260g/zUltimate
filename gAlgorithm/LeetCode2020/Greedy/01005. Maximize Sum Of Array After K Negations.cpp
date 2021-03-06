// K 次取反后最大化的数组和
// 给定一个整数数组 A，我们只能用以下方法修改该数组：
// 我们选择某个索引 i 并将 A[i] 替换为 -A[i]，
// 然后总共重复这个过程 K 次。（我们可以多次选择同一个索引 i。）
// 以这种方式修改数组后，返回数组可能的最大和。

// 输入：A = [4,2,3], K = 1
// 输出：5
// 解释：选择索引 (1,) ，然后 A 变为 [4,-2,3]。

// 输入：A = [3,-1,0,2], K = 3
// 输出：6
// 解释：选择索引 (1, 2, 2) ，然后 A 变为 [3,1,0,2]。

// 输入：A = [2,-3,-1,5,-4], K = 2
// 输出：13
// 解释：选择索引 (1, 4) ，然后 A 变为 [2,3,-1,5,4]。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/delete-columns-to-make-sorted
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


// 1. 排序
// 2. 遍历
// 2.1 如果还有翻转次数 且 A[i] < 0；那就翻转后累加
// 2.2 否则; 直接累加
// 2.3 翻转过程中; 记录绝对值最小的索引
// 3. 如果还剩下 奇数次翻转
// 3.1 那就把绝对值最小的数还原 再 累加
class Solution {
public:
    int largestSumAfterKNegations(vector<int>& A, int K) {
        // 1. 如果 A[] 中的负数个数小于 K
        //    那么尽可能地把较大的负数 转换为正数
        // 2. 如果 A[] 中的已经全部为正数 
        //    那么检查 K 的奇偶
        //    K 为偶数; 则不再做翻转  （可以连续翻转同一个数 消耗K)
        //    K 为奇数; 找到最小的翻转
        sort(A.begin(), A.end());
        int sum = 0, min_abs_idx = 0;
        for (int i = 0; i < A.size(); ++i) {
            // 还有翻转次数; 那就把较小的负数翻转
            if (K > 0 && A[i] < 0) { 
                sum += -A[i];
                --K;
            } else {
                sum += A[i];
            }
            if (abs(A[i]) < abs(A[min_abs_idx]))
                min_abs_idx = i;
        }

        // '2*A[min_abs_idx]' 有两层
        // 先把之前累加的结果还原
        // 再把原始地数值累加进去
        if (K & 0x1) { // 还有翻转次数
            if (A[min_abs_idx] < 0)
                sum += 2*A[min_abs_idx];
            else
                sum -= 2*A[min_abs_idx];
        }
        return sum;
    }
};