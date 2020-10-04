// 在给定二叉树中 搜索目标值的节点;
// 返回节点; 否则返回空节点

// 递归法
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root) return root;  
        if (val < root->val)
            return searchBST(root->left, val);
        if (root->val < val)
            return searchBST(root->right, val);
        return root;
    }
};

// 迭代法
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root) return root;
        while (root) {
            if (root->val == val)
                return root;
            else if (val < root->val)
                root = root->left;
            else
                root = root->right;
        }
        return nullptr;
    }
};
