// Given the root of a binary search tree and the lowest and highest boundaries as low and high, 
// trim the tree so that all its elements lies in [low, high].
// You might need to change the root of the tree,
// so the result should return the new root of the trimmed binary search tree.
//
// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/trim-a-binary-search-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。
//
// BST 减枝; 给定数值范围；干掉不在范围内的节点
class Solution {
public:
    TreeNode* trimBST(TreeNode* root, int low, int high) {
        if (!root)  return root;
        if (root->left)
            root->left = trimBST(root->left, low, high);
        if (root->right)
            root->right = trimBST(root->right, low, high);
        if (root->val < low || root->val > high) {
            // 单枝or叶节点
            if (!root->right)
                return root->left;
            if (!root->left)
                return root->right;
            // 由于是BST后序遍历; 
            // 1. 如果当前节点 小于low 那么遍历到这里 左子树 一定是nullptr
            // 2. 如果当前节点 大于high那么遍历到这里 右子树 一定是nullptr
            // 因此; 如果当前节点不在范围内部; 那么后续遍历到这里一定是单枝的
        }
        return root;
    }
};