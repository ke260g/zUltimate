// Given an array nums with n objects colored red, white, or blue, 
// sort them in-place so that objects of the same color are adjacent,
// with the colors in the order red, white, and blue.
// Here, we will use the integers 0, 1, and 2 to represent the color red, white, and blue respectively.

// Follow up:
//     Could you solve this problem without using the library's sort function?
//     Could you come up with a one-pass algorithm using only O(1) constant space?

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/sort-colors
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// Input: nums = [2,0,2,1,1,0]
// Output: [0,0,1,1,2,2]

// 经典算法问题: 荷兰国旗问题; 有算法论文的
// 双指针法; 确定三个左开右闭区间;
// 1. 遍历过程中, 确保变量的定义不变
// 2. 变量定义的初始化数值正确设置
// 3. 交换和移动指针的先后顺序
class Solution {
public:
    void sortColors(vector<int>& nums) {
        // 0: [0, one)    变量 one 表示  小于one 的索引都是 0
        // 1: [one, i)    大于等于 one 且小于 i 的索引都是 1
        // ?: [i, two)    这部分是未排序的
        // 2: [two, nums.size()) 大于等于 two 的索引都是   2
        int two = nums.size(), // 因为 索引等于two的 必然是2; 但初始时不确定末尾元素是多少
            one = 0, // 不确定 开头元素时1还是其他; 假设 (-无穷, one) 都是 0; 所以设one为0
            i = 0;
        while (i < two) {
            if (nums[i] == 0)
                swap(nums[i++], nums[one++]);
            else if (nums[i] == 1)
                i++;
            else if (nums[i] == 2)
                swap(nums[i], nums[--two]);
            else
                return;
        }
        return;
    }
};