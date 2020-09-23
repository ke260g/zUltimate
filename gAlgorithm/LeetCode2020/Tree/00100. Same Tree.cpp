/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    bool result = true;
    bool isSameTree(TreeNode* p, TreeNode* q) {
        // 前序遍历法
        // 1. 两个空节点是相同,
        // 2. 只有一个是空节点 不相同
        // 3. 先判断左子树, 是否相同
        // 4. 再判断右子树, 是否相同
        // 5. 最后判断当前节点是否相同
        // 6. 优化; 全局结果; 提前退出递归
        if (!result) return false;
        if (!p && !q) return true;
        if (!p || !q) return false;
        if (!isSameTree(p->left, q->left)) {
            result = false;
            return false;
        }
        if (!isSameTree(p->right, q->right)) {
            result = false;
            return false;
        }
        return p->val == q->val;
    }
};