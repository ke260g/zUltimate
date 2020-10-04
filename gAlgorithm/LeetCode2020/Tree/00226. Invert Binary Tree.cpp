// Input:
//      4
//    /   \
//   2     7
//  / \   / \
// 1   3 6   9

// Output:
//      4
//    /   \
//   7     2
//  / \   / \
// 9   6 3   1
// 翻转二叉树
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return root;
        invertTree(root->left);
        invertTree(root->right);
        swap(root->left, root->right);
        return root;
    }
};