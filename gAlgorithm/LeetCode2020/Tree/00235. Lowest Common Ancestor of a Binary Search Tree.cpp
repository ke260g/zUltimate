// Given a binary search tree (BST), find the lowest common ancestor (LCA) of two given nodes in the BST.

// According to the definition of LCA on Wikipedia: 
// “The lowest common ancestor is defined between two nodes p and q 
// as the lowest node in T that has both p and q as descendants 
// (where we allow a node to be a descendant of itself).”

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/lowest-common-ancestor-of-a-binary-search-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 在二叉搜索树中找到 最近公共祖先
// 1. 如果两节点都大于 当前节点; 那么公共祖先必然在该节点的右子树中
// 2. 如果两节点都小于 当前节点; 那么公共祖先必然在该节点的左子树中
// 3. 否则; 两节点值分布在当前节点的两边; 那么当前节点就是最近的公共祖先
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root) return root;
        if (p->val < root->val && q->val < root->val)
            return lowestCommonAncestor(root->left, p, q);
        if (root->val < p->val && root->val < q->val)
            return lowestCommonAncestor(root->right, p, q);
        return root;
    }
};