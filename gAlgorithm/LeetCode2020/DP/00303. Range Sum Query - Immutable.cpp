// Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.

// Given nums = [-2, 0, 3, -5, 2, -1]

// sumRange(0, 2) -> 1
// sumRange(2, 5) -> -1
// sumRange(0, 5) -> -3

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/range-sum-query-immutable
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 经典的数组前缀和应用; 
// 因为要求左闭右闭; 所以索引有一些变化
// prev_nums[j] - prev_nums[i-1]
// 列个表出来; 展示一下几个元素即可
class NumArray {
    vector<int> sums;
public:
    NumArray(vector<int>& nums) {
        if (nums.size() == 0) return;
        sums.assign(nums.size(), 0);
        // sums[0] = nums[0]
        // sums[1] = sums[0] + sums[1]
        // sums[2] = sums[0] + sums[1] + sums[2]
        // sums[3] = sums[0] + sums[1] + sums[2] + sums[3]
        sums[0] = nums[0];
        for (int i = 1; i < nums.size(); ++i)
            sums[i] += sums[i-1] + nums[i];
    }
    
    int sumRange(int i, int j) {
        if (sums.empty()) return 0;
        return i == 0 ? sums[j] : sums[j] - sums[i-1];
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(i,j);
 */