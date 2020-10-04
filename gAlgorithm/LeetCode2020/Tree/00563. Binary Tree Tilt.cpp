// 二叉树的坡度; 定义为: 每个非叶节点的左右子树数值和的差的绝对值 的 总和
// 后续遍历; 先计算左右节点的数值和; 再把 数值和的差的绝对值 累加到结果中
class Solution {
public:
    int valueOfTree(TreeNode *root, int &res) {
        if (!root) return 0;
        int valueOfLeft = valueOfTree(root->left, res);
        int valueOfRight = valueOfTree(root->right, res);
        res += abs(valueOfLeft - valueOfRight);
        return valueOfLeft + valueOfRight + root->val;
    }
    int findTilt(TreeNode* root) {
        int res = 0;
        valueOfTree(root, res);
        return res;
    }
};