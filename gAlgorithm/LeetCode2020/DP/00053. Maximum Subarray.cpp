// Given an integer array nums, find the contiguous subarray (containing at least one number) 
// which has the largest sum and return its sum.

// Follow up: If you have figured out the O(n) solution,
// try coding another solution using the divide and conquer approach, 
// which is more subtle.

// Input: nums = [-2,1,-3,4,-1,2,1,-5,4]
// Output: 6
// Explanation: [4,-1,2,1] has the largest sum = 6.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/range-sum-query-immutable
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


// 官方的参考答案
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int res = nums[0], local_sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            // 1. 针对每一个数值
            // 2. 该数值前的子数组和为负;
            //    那么新的 local_sum 从当前数值重新开始
            // 3. 该数值前的子数组和非负; 那就加入 local_sum表示连续子数组
            // 4. 在局部local_sum更新后; 都与res结果对比
            if (local_sum < 0)
                local_sum = nums[i];
            else
                local_sum += nums[i];
            res = max(local_sum, res);
        }
        return res;
    }
};

// 自己脑洞想出来的
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if (nums.size() == 1) return nums[0];
        vector<int> sums(nums.size()+1, 0);

        sums[1] = nums[0];
        // sums[0] = 0
        // sums[1] = nums[0]
        // sums[2] = nums[0] + nums[1]
        // sums[3] = nums[0] + nums[1] + nums[2]

        // 1. 求出前缀和的数组 sums[i] 表示 0~i 的加和
        // 2. sums[j] - sums[i] 表示 i+1~j 的加和
        // 3. 最大连续子数组即可 sums[j]-sums[i] 值最大
        // 4. 对于任意一个 sums[i] 
        //    要么是 sums[i] 较大 or 要么是 sums[i] - sums[j] 较大
        //    其中sums[j]是 0~(i-1) 中的最小值
        // 5. 然后取上步走中的 较大值与当前最大值res比较; 更新
        // 实际上; 在sums序列中会出现多个局部最大值和局部最小值
        // 一对连续的 局部最小值和局部最大值
        for (int i = 1; i < sums.size(); ++i)
            sums[i] = sums[i-1]+nums[i-1];

        int res = sums[1];
        int local_min = sums[1];
        for (int i = 2; i < sums.size(); ++i) {
            int local_max = max(sums[i], sums[i] - local_min);
            if (res < local_max)
                res = local_max;
            if (sums[i] < local_min)
                local_min = sums[i];
        }
        return res;
    }
};
