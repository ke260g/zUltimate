//  Given a binary tree, you need to compute the length of the diameter of the tree. 
//  The diameter of a binary tree is the length of the longest path between any two nodes in a tree. 
//  This path may or may not pass through the root. 

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/diameter-of-binary-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

//       1
//      / \
//     2   3
//    / \     
//   4   5

class Solution {
public:
    int treeDepth(TreeNode *root, int *res) {
        // 1. 后续遍历; 求左右子树的高度
        // 2. 一个子树经过根节点的直径长度 = 左子树高度+右子树高度
        // 3. 但是存在子树的直径长度 > 经过根节点的直径长度
        // 4. 因此必须在后续遍历后 根据根节点的直径长度 与 已知最大直径长度比较; 更新
        if (!root) return 0;
        int depthOfLeft = treeDepth(root->left, res);
        int depthOfRight = treeDepth(root->right, res);
        int diameter = depthOfLeft + depthOfRight;
        if (*res < diameter)
            *res = diameter;
        return max(depthOfLeft, depthOfRight) + 1;
    }
    int diameterOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        int res = 0;
        treeDepth(root, &res);
        return res;
    }
};