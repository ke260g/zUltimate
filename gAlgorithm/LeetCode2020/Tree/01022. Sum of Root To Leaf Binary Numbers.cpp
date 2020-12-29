// 计算二进制二叉树 从根到每个叶节点路径上的值 的和
// 每一条路径上根表示最高位


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
    int calcFromPath(vector<bool> &path) {
        int res = 0;
        for (int i = 0; i < path.size(); ++i)
            res = res*2 + (path[i] ? 1 : 0);
        return res;
    }
    void sumRootToLeaf(TreeNode* root, vector<bool> &path, int &res) {
        if (!root) return;
        // 1. 空节点直接返回
        // 2. 叶节点把当前节点的值塞进路径中
        // 3.1 如果是叶节点; 则递归终止 > 求出路径值
        // 3.2 否则分别遍历左右子树
        // 4. 最后把自己从路径中弹出
        path.push_back(root->val);
        if (!root->left && !root->right) {
            res += calcFromPath(path);
        } else {
            sumRootToLeaf(root->left, path, res);
            sumRootToLeaf(root->right, path, res); 
        }
        path.pop_back();
    }
    int sumRootToLeaf(TreeNode* root) {
        int res = 0;
        vector<bool> path;
        sumRootToLeaf(root, path, res);
        return res;
    }
};