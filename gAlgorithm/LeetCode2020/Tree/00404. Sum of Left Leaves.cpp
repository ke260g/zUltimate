// 计算左叶节点的总和
//     3
//    / \
//   9  20
//     /  \
//    15   7
// There are two left leaves in the binary tree, with values 9 and 15 respectively. Return 24.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/sum-of-left-leaves
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        // 1. 空树的值为0
        // 2. 如果存在左节点;
        // 2.1 左节点是叶; 结果加上左节点的值
        // 2.2 左节点非叶; 结果加上左子树的值(递归)
        // 3. 如果存在右节点; 结果加上右子树的值(递归)
        if (!root) return 0;
        int res = 0;
        if (root->left) {
            if (!root->left->left && !root->left->right)
                res += root->left->val;
            else
                res += sumOfLeftLeaves(root->left);
        }
        if (root->right)
            res += sumOfLeftLeaves(root->right);
        return res;
    }
};