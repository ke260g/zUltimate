// Given a binary tree, return all root-to-leaf paths.
// Input:
//    1
//  /   \
// 2     3
//  \
//   5
// Output: ["1->2->5", "1->3"]

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
    void binaryTreePathsEx(TreeNode* root, vector<string> & res, string &path) {
        // 1. 空节点直接返回
        // 2. 叶节点把值加到路径后; 然后最终路径结果加到res中
        // 3. 左子树非空; 那就把当前节点加到路径path中; 在加上"->"标记; 递归左子树
        // 4. 右子树非空; 那就把当前节点加到路径path中; 在加上"->"标记; 递归右子树
        if (!root)
            return;
        if (!root->left && !root->right) {
            string npath(path);
            npath += to_string(root->val);
            res.push_back(npath);
        }
        if (root->left) {
            string lpath(path);
            lpath += to_string(root->val);
            lpath += "->";
            binaryTreePathsEx(root->left, res, lpath);
        }
        if (root->right) {
            string rpath(path);
            rpath += to_string(root->val);
            rpath += "->";
            binaryTreePathsEx(root->right, res, rpath);
        }
    }
    vector<string> binaryTreePaths(TreeNode* root) {
        vector<string> res;
        string path;
        binaryTreePathsEx(root, res, path);
        return res;
    }
};