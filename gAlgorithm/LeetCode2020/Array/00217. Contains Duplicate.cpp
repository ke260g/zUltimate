// Given an array of integers,
// find if the array contains any duplicates.

// Your function should return true if any value appears at least twice in the array, 
// and it should return false if every element is distinct.

// Input: [1,2,3,1]
// Output: true

// Input: [1,2,3,4]
// Output: false

// Input: [1,1,1,3,3,4,3,2,4,2]
// Output: true

// 方法1: hash记录之前访问过的元素
//        每个新方访问的元素都 先从 hash 中查找一下
//        找到了说明有重复
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        unordered_set<int> unum;
        for (int n: nums) {
            if (unum.find(n) != unum.end()) {
                return true;
            }
            unum.insert(n);
        }
        return false;
    }
};

// 方法2: 排序, 检查是否存在连续两个相同的元素
class Solution {
public:
    bool containsDuplicate(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == nums[i-1])
                return true;
        }
        return false;
    }
};