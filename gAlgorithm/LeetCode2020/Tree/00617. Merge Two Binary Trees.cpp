/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {
        // 1. 两树为空; 返回空
        // 2.  t1 不空; t2为空; 返回t1
        // 3.  t1 为空；t2不空; 返回t2
        // 3. t1 t2 都不空; 分别合并两者的左右子树; 合并节点本身的值
        if (!t2) return t1;
        if (!t1) return t2;
        t1->left = mergeTrees(t1->left, t2->left);
        t1->right = mergeTrees(t1->right, t2->right);
        t1->val += t2->val;
        return t1;
    }
};