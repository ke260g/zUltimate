// 给定两个 没有重复元素 的数组 nums1 和 nums2 ，其中nums1 是 nums2 的子集。
// 找到 nums1 中每个元素在 nums2 中的下一个比其大的值。
// nums1 中数字 x 的下一个更大元素是指 x 在 nums2 中对应位置的右边的第一个比 x 大的元素。如果不存在，对应位置输出 -1 。

// 输入: nums1 = [4,1,2], nums2 = [1,3,4,2].
// 输出: [-1,3,-1]
// 解释:
//     对于num1中的数字4，你无法在第二个数组中找到下一个更大的数字，因此输出 -1。
//     对于num1中的数字1，第二个数组中数字1右边的下一个较大数字是 3。
//     对于num1中的数字2，第二个数组中没有下一个更大的数字，因此输出 -1。

// 输入: nums1 = [2,4], nums2 = [1,2,3,4].
// 输出: [3,-1]
// 解释:
//     对于 num1 中的数字 2 ，第二个数组中的下一个较大数字是 3 。
//     对于 num1 中的数字 4 ，第二个数组中没有下一个更大的数字，因此输出 -1 。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/next-greater-element-i
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 题目的本质是再O(1) 复杂度下; 构造一个hash
// hash[nums2[i]] = 下一个较大的元素

class Solution {
public:
    vector<int> nextGreaterElement(vector<int>& nums1, vector<int>& nums2) {
        // 1. 维护一个单调递减的栈 (栈中的元素即还没有找到 下一个比ta大地元素)
        // 2. 当前元素比 栈顶要大; 则栈顶元素找到下一个较大元素;
        // 3. 出栈并把结果缓存起来hash
        // 4. 找完之后; 当前元素继续入栈
        // 5. 遍历nums1, hash[nums1[i]] 如果能找到则表示下一个较大的元素值  
        map<int, int> hash_next_greater; // key: nums2[i] value: nums2[j]下一个较大的元素
        stack<int> decreaseStack;
        for (auto it = nums2.begin(); it != nums2.end(); ++it) {
            // 2. 当前元素比 栈顶要大; 则栈顶元素找到下一个较大元素;
            while (!decreaseStack.empty() && decreaseStack.top() < *it) {
                // 3. 出栈并把结果缓存起来hash
                hash_next_greater[decreaseStack.top()] = *it;
                decreaseStack.pop();
            }
            // 找完之后; 当前元素继续入栈
            decreaseStack.push(*it);
        }
        vector<int> res(nums1.size(), -1); // 默认找不到
        for (int i = 0; i < nums1.size(); ++i) {
            auto found_result = hash_next_greater.find(nums1[i]);
            if (found_result != hash_next_greater.end())
                res[i] = found_result->second;
        }
        return res;
    }
};