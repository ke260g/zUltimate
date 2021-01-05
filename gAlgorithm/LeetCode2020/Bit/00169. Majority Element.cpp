// Given an array of size n, find the majority element.
// The majority element is the element that appears more than ⌊ n/2 ⌋ times.

// You may assume that the array is non-empty and the majority element always exist in the array.

// Input: [3,2,3]
// Output: 3

// Input: [2,2,1,1,1,2,2]
// Output: 2

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/majority-element
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 这一题其他方法有
// 1. 排序
// 2. hash
// 但是; 符合 bit 操作的只有摩尔投票法

// 摩尔投票法的经典应用
class Solution {
public:
    int majorityElement(vector<int>& nums) {
        int ans = nums[0], cnt = 1;
        // 1. 计数器为0; 那就选择当前值 为候选人
        // 2. 当前值 == 候选人; 有人站队; 队伍更大; 计数器++
        // 3. 当前值 != 候选人; 有人敌对; 队伍缩小(拿队友去送); 计数器--
        for (int i = 1; i < nums.size(); ++i) {
            if (cnt == 0)
                ans = nums[i];
            cnt += (ans == nums[i]) ? 1 : -1;
        }
        return ans;
    }
};