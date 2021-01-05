// Given an array of integers and an integer k, 
// find out whether there are two distinct indices i and j in the array such that nums[i] = nums[j] 
// and the absolute difference between i and j is at most k.

// Input: nums = [1,2,3,1], k = 3
// Output: true

// Input: nums = [1,0,1,1], k = 1
// Output: true

// Input: nums = [1,2,3,1,2,3], k = 2
// Output: false

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/contains-duplicate-ii
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 题意: 给定数组 和 k; 检查数组中是否存在相同元素; 他们的索引距离不大于k


// 自己的做法:
// 1. 记录 hash { k: value, v: index }
// 2. 遍历元素时; 如果之前有相同元素; 则计算他们的 索引距离
//    满足条件则返回 true
// 3. 每个元素遍历完都需要更新 hash
//    因为要求 距离不大于k; 所以索引距离尽可能短;
//    因此; 更新当前元素的index; 使得下次遇到相同元素时; 索引距离更短
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        map<int, int> memo; // k:value, v:index
        for (int i = 0; i < nums.size(); ++i) {
            auto prev = memo.find(nums[i]);
            if ((prev != memo.end()) && (i - prev->second <= k))
                return true;
            memo[nums[i]] = i;
        }
        return false;
    }
};