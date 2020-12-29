// 按摩师问题; 等同打劫问题(00198. House Robber 原题)
// 数学本质: 在给定非负数列中, 选取一个不连续子数列, 子数列元素在原数列中不能两相邻
// 求数列和最大的子数列以及ta数列和
// A popular masseuse receives a sequence of back-to-back appointment requests and is debating which ones to accept. 
// She needs a break between appointments and therefore she cannot accept any adjacent requests. 
// Given a sequence of back-to-back appoint­ ment requests, 
// find the optimal (highest total booked minutes) set the masseuse can honor.
// Return the number of minutes.
//
// Note: This problem is slightly different from the original one in the book.
//
// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/the-masseuse-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// Input:  [1,2,3,1]
// Output:  4
// Explanation: [1, 3]
class Solution {
public:
    int massage(vector<int>& nums) {
        if (nums.empty()) return 0;
        if (nums.size() == 1) return nums[0];
        if (nums.size() == 2) return max(nums[0], nums[1]);
        if (nums.size() == 3) return max(nums[1], nums[0]+nums[2]);
        // 00198. House Robber 原题
        vector<int> order(3, 0);
        order[0] = nums[0];
        order[1] = nums[1];
        order[2] = nums[0] + nums[2];
        for (int i = 3; i < nums.size(); ++i) {
            int curr = max(order[0], order[1]) + nums[i];
            memmove(order, order+1, 2*sizeof(int));
            order[2] = curr;
        }
        return max(order[2], order[1]);   
    }
};