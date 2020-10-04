// A binary tree is univalued if every node in the tree has the same value.
// Return true if and only if the given tree is univalued.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/univalued-binary-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    bool isUnivalTree(TreeNode* root) {
        // 单值二叉树的定义
        // 1. 当前节点的值等于 非空子节点的值
        // 2. 非空子树分别都是 单值二叉树
        if (!root) return true;
        if (root->left && root->val != root->left->val)
            return false;
        if (root->right && root->val != root->right->val)
            return false;
        return isUnivalTree(root->left) && isUnivalTree(root->right);
    }
};