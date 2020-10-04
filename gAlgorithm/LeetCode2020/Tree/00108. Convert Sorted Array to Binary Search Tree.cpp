// 题目要求给定排序数组构造成平衡二叉树
// 不需要使用avl的;
// Input: [-10,-3,0,5,9],
// Output:
//       0
//      / \
//    -3   9
//    /   /
//  -10  5



/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
// 本质是前序遍历
class Solution {
public:
    TreeNode *insertToBST(vector<int>&nums, int head, int tail) {
        if (head > tail) return NULL;
        // 1. 规定任意子数组; 子数组中间元素作为树的根
        // 2. 子数组开始到中间元素-1 作为左子树 (递归)
        // 3. 子数组中间元素+1到结尾 作为右子树 (递归)
        int mid = (tail + head) / 2;
        TreeNode *root = new TreeNode(nums[mid]);
        root->left = insertToBST(nums, head, mid-1);
        root->right = insertToBST(nums, mid+1, tail);
        return root;
    }
    TreeNode* sortedArrayToBST(vector<int>& nums) {
        if (nums.empty()) return NULL;
        return insertToBST(nums, 0, nums.size()-1);
    }
};