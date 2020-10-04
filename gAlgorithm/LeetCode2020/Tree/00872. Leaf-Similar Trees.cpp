// 给定两棵二叉树; 判断他们的 叶节点从左往右的序列是否完全相等

// 后序遍历得到叶序列
// 方法1: 分别用计算两个树的叶序列; 再判断两个树的叶序列是否相等
class Solution {
public:
    void leavesCaches(TreeNode* root, vector<int> &res) {
        if (!root) return;
        leavesCaches(root->left, res);
        leavesCaches(root->right, res);
        if (!root->left && !root->right) {
            res.push_back(root->val);
            return;
        }
    }
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> res1, res2;
        leavesCaches(root1, res1);
        leavesCaches(root2, res2);
        if (res1.size() != res2.size())
            return false;
        for (int i = 0; i < res1.size(); ++i) {
            if (res1[i] != res2[i])
                return false;
        }
        return true;
    }
};



// 方法2: 先后序遍历(左右根)缓存一个树的叶序列;
//        再"反后序遍历(右左根)"另一个树; 把相等 叶节点相同的 res.back() 销毁调
// 如果 res 最后被清空; 说明两树的叶节点序列相同
class Solution {
public:
    void leavesPush(TreeNode* root, vector<int> &leaves) {
        if (!root) return;
        leavesPush(root->left, leaves);
        leavesPush(root->right, leaves);
        if (!root->left && !root->right) {
            leaves.push_back(root->val);
            return;
        }
    }
    void leavesPop(TreeNode* root, vector<int> &leaves, bool & res) {
        if (!root) return;
        if (!res) return;
        leavesPop(root->right, leaves, res);
        leavesPop(root->left, leaves, res);
        if (!root->left && !root->right) {
            if (root->val == leaves[leaves.size()-1]) {
                leaves.pop_back();
            } else {
                res = false;
            }
            return;
        }
    }
    bool leafSimilar(TreeNode* root1, TreeNode* root2) {
        vector<int> leaves;
        bool res = true;
        leavesPush(root1, leaves);
        leavesPop(root2, leaves, res);
        return leaves.empty() ? true : false;
    }
};