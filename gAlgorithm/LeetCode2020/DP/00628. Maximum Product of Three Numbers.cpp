class Solution {
public:
    int maximumProduct(vector<int>& nums) {
        // 先排序; 取最大的3个数值; 这样慢了; 需要 log2n
        // dp 一次求得最大的3个数值
        int res = 1;
        if (nums.size() < 4) {
            for (int i = 0; i < nums.size(); ++i)
                res *= nums[i];
            return res;
        }
        res = 1;
        int head[3] = {nums[0], nums[1], nums[2]};
        vector<int> max(head, head+3);
        sort(max.begin(), max.end());
        vector<int> min(max);
        min.pop_back();
        for (int i = 3; i < nums.size(); ++i) {
            if (max[2] < nums[i]) {
                max[0] = max[1]; max[1] = max[2]; max[2] = nums[i];
            }
            else if (max[1] < nums[i] && nums[i] <= max[2]) {
                max[0] = max[1]; max[1] = nums[i];
            }
            else if (max[0] < nums[i] && nums[i] <= max[1])
                max[0] = nums[i];

            if (nums[i] < min[0]) {
                min[1] = min[0]; min[0] = nums[i];
            } else if (min[0] <= nums[i] && nums[i] < min[1])
                min[1] = nums[i];
        }
        int res1 = max[0] * max[1] * max[2];
        int res2 = min[0] * min[1] * max[2];
        return res1 > res2 ? res1 : res2;

        // 动态规划; 找到最大的3个值和最小的2个值 (注意大于等于 和 小于等于的场景)
        // 1. 全是正数or全负数: 最大的三个数的积
        // 2. 最小的两个负数积*最大的一个正数积
        // 上述两种场景求较大值
    }
};

