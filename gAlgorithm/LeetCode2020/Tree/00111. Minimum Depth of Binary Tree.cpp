// Given a binary tree, find its minimum depth.
// The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
// Note: A leaf is a node with no children.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/minimum-depth-of-binary-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// Input:
//     3
//    / \
//   9  20
//     /  \
//    15   7
// Output: 2

// Input:
//   1
//  /
// 2
// Output: 2

class Solution {
public:
    int minDepth(TreeNode* root) {
    // 题目的意思需要包含子叶;
    // 1. 空树的高度为0
    // 2. 单枝树的最小高度 为非空子树的最小高度+1
    //    这个条件不能少; 不然单枝子树会误判位 深度0
    // 3. 双枝树的最小高度 为两子树的最小高度的较少值+1
        if (!root) return 0;
        if (!root->left)
            return minDepth(root->right)+1;
        if (!root->right)
            return minDepth(root->left)+1;
        return min(minDepth(root->left), minDepth(root->right))+1;
    }
};