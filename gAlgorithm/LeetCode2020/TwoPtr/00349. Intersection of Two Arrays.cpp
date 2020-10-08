// Given two arrays, write a function to compute their intersection.

// Input: nums1 = [1,2,2,1], nums2 = [2,2]
// Output: [2]

// Input: nums1 = [4,9,5], nums2 = [9,4,9,8,4]
// Output: [9,4]

// Note:
//     Each element in the result must be unique.
//     The result can be in any order.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/intersection-of-two-arrays
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 求两数组的交集; 交集中元素不可重复; 但输入数组没有单值化
class Solution { // 自己写的双80% 答案
public:
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        // 1. 排序
        // 2. 双指针法: 指针j 和 指针i 定义为 nums1[i] < nums2[j]
        // 3.1 如果相等 则加入到交集; (加入时判断是否重复; 因为已排序 只需要判断交集当前的最大值是否相等即可)
        // 3.2 nums1[i] < nums2[j] 符合约定: i++; 即i指向更大的数
        // 3.3 nums1[i] > nums2[j] 约定不合; 尝试继续保证约定; 使得j指向更大地数; ++j
        vector<int> intersection;
        
        sort(nums1.begin(), nums1.end());
        sort(nums2.begin(), nums2.end());

        int i = 0, j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            // 3.1
            if (nums1[i] == nums2[j]) {
                if (intersection.empty() || nums1[i] != intersection[intersection.size()-1])
                    intersection.push_back(nums1[i]);
                ++i;
                ++j;
            }
            else if (nums1[i] < nums2[j]) // 3.1
                ++i;
            else if (nums1[i] > nums2[j]) // 3.2
                ++j;
        }
        return intersection;
    }
};