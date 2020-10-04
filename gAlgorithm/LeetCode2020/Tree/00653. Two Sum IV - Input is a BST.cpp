// BST 树找到两节点的和 为给定的数
// 能找到返回true

// 暴力求解; 中序遍历; 用hash缓存每一个已访问元素的值
class Solution {
public:
    void findTargetEx(TreeNode* root, int k, map<int, bool> &visited, bool &res) {
        if (res) return; // 提前结束
        if (!root) return;
        findTargetEx(root->left, k, visited, res);
        if (visited.find(k - root->val) != visited.end()) {
            res = true;
            return;
        }
        visited[root->val] = true;
        findTargetEx(root->right, k, visited, res);
    } 
    bool findTarget(TreeNode* root, int k) {
        bool res = false;
        map<int, bool> visited;
        findTargetEx(root, k, visited, res);
        return res;
    }
};