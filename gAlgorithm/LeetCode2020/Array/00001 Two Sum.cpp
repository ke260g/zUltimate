class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> hash; // key: nums[i], value:i;
        vector<int> res;
        for(int i = 0; i < nums.size(); ++i) {
            auto remain = hash.find(target - nums[i]);
            if (remain != hash.end()) {
                res.push_back(remain->second);
                res.push_back(i);
                break;
            } else {
                hash[nums[i]] = i;
            }
        }
        return res;
    }
};