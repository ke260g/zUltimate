// Given the root node of a binary search tree, return the sum of values of all nodes with value between L and R (inclusive).

// Input: root = [10,5,15,3,7,null,18], L = 7, R = 15
// Output: 32
// Explanation: [ 7,10,15 ]

// Input: root = [10,5,15,3,7,13,18,1,null,6], L = 6, R = 10
// Output: 23
// Explanation: [ 6,7,10 ]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/range-sum-of-bst
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    int rangeSumBST(TreeNode* root, int L, int R) {
        // 空节点 sum 为0
        // 当前节点 > R；当前节点的值不能加和; 返回左子树的加和
        // 当前节点 < L; 当前节点的值不能加和; 返回右子树的加和
        // L <= root->val <= R 返回左右子树+节点的加和
        if (!root) return 0;
        // 中序遍历
        if (R < root->val)
            return rangeSumBST(root->left, L, R);
        if (root->val < L)
            return rangeSumBST(root->right, L, R);
        // L <= root->val <= R
        return root->val + rangeSumBST(root->left, L, R) + rangeSumBST(root->right, L, R);
    }
};