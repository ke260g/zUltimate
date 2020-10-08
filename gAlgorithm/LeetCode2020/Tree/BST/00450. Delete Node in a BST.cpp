// 删除BST的节点值
// 1. 待删除节点是叶节点; 直接删除
// 2. 待删除节点是单枝节点; 删除后返回其树枝
// 3. 待删除节点有左右子树;
// 3.1 替代后继节点;即 root->right->left->left-left
// 3.2 重新调用这个函数; 删除右子树中的后继节点
//
// Note: 实际上; 删除一个BST过程中; 如果待删除节点有左右子树
//       则需要先用后继节点替换; 再删除后继节点(后继节点一定是 单枝or叶子)
//       一次删除函数回被调用两次
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;
        if (key == root->val) {
            // 1. 叶节点直接删除
            if (!root->left && !root->right)
                return nullptr;
             // 2. 单枝节点返回剩余地单枝
            if (!root->left) return root->right;
            if (!root->right) return root->left;
            
            // 3. 同时有左右子树
            // 3.1 替代后继节点
            // 3.2 删除后继节点
            TreeNode *t = root->right;
            while (t->left)
                t = t->left;
            root->val = t->val;
            // root->left = t->left; // 后继节点一定没有左子树
            root->right = deleteNode(root->right, t->val);
        } else if (key < root->val) {
            // 4. 待删除节点小于根节点; 则往左子传递
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            // 4. 待删除节点大与根节点; 则往右子传递
            root->right = deleteNode(root->right, key);
        }
        return root;
    }
};