/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */

// 给定一个二叉树和一个目标和，判断该树中是否存在根节点到叶子节点的路径，这条路径上所有节点值相加等于目标和。
// 说明: 叶子节点是指没有子节点的节点。

// Input:
//       5
//      / \
//     4   8
//    /   / \
//   11  13  4
//  /  \      \
// 7    2      1
// sum = 22
// Output: true
// Explanation: 5->4->11->2
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        // 1. 空树为false (因此不能把空的节点进行递归)
        // 2. 叶节点判断 val 和 sum
        // 3. 非叶节点则 把sum减去当前节点的值; 分别遍历左右"非空"子树
        // 4. 任一子树满足条件; 则该子树满足条件
        if (!root) return false;
        if (!root->left && !root->right)
            return sum == root->val;
        return (root->left && hasPathSum(root->left, sum - root->val)) ||
               (root->right && hasPathSum(root->right, sum - root->val));
    }
};