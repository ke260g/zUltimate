// Given a Binary Search Tree (BST) with the root node root,
// return the minimum difference between the values of any two different nodes in the tree.
//
// Input: root = [4,2,6,1,3,null,null]
// Output: 1
// Explanation:
// Note that root is a TreeNode object, not an array.
//
// The given tree [4,2,6,1,3,null,null] is represented by the following diagram:
//
//           4
//         /   \
//       2      6
//      / \    
//     1   3  
//
// while the minimum difference in this tree is 1, it occurs between node 1 and node 2, also between node 3 and node 2.
//
// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/minimum-distance-between-bst-nodes
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    void minDiffInBSTEx(TreeNode *root, TreeNode **prev, int *res) {
        if (!root) return;
        minDiffInBSTEx(root->left, prev, res);
        if (*prev) { // 前驱节点非空; 则更新 diff
            int diff = root->val - (*prev)->val;
            *res = min(diff,  *res);
        }
        *prev = root; // 更新前驱节点
        minDiffInBSTEx(root->right, prev, res);
    }
    int minDiffInBST(TreeNode* root) {
        // 中序遍历, 缓存res和上一个节点
        TreeNode *prev = NULL;
        int res = INT_MAX;
        minDiffInBSTEx(root, &prev, &res);
        return res;
    }
};