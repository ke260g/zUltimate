 /*
判断是否对称
    1
   / \
  2   2
 / \ / \
3  4 4  3
 */

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
    bool isSymmetric(TreeNode* root) {
        // 1. 空树是对称的
        // 2. 用两个队列; 一个用来存左子树; 一个用来存右子树
        // 3. 根部左节点、右节点分别入列;
        // 4. 对于 左树队列 和 右树队列同时遍历
        // 4.1 两节点同时为空    continue; 对称条件
        // 4.2 任意节点非空      return 不对称
        // 4.3 节点值不相等      return 不对称
        // 4.3 左子树队列节点的left 和 右子树队列节点的right 分别入列
        // 4.4 左子树队列节点的right 和 右子树队列节点的left 分别入列
        // 5. 如果4.步骤遍历后; 如果左右子树队列任意非空; 表明不对称
        if (!root) return true;
        queue<TreeNode *> X, Y;
        X.push(root->left);
        Y.push(root->right);
        while (!X.empty() && !Y.empty()) {
            TreeNode *x = X.front(); X.pop();
            TreeNode *y = Y.front(); Y.pop();
            if (!x && !y) continue;
            if (!x || !y) return false;
            if (x->val != y->val) return false;
            X.push(x->left);
            Y.push(y->right);

            X.push(x->right);
            Y.push(y->left);
        }
        if (!X.empty() || !Y.empty())
            return false;
        return true;
    }
};