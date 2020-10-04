// 给定一个二叉树，找到最长的路径，这个路径中的每个节点具有相同值。
// 这条路径可以经过也可以不经过根节点。
// 注意：两个节点之间的路径长度由它们之间的边数表示。

// 示例 1:
// 输入:
//
//               5
//              / \
//             4   5
//            / \   \
//           1   1   5
// 输出: 2

// 示例 2:
// 输入:
//               1
//              / \
//             4   5
//            / \   \
//           4   4   5
// 输出: 2

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/longest-univalue-path
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 根据题目的意思: 路径长度 = 节点个数-1
// 1. 后续遍历
// 2. 计算经过当前节点的单值路径长度
class Solution {
    int sameValueCount(TreeNode *root, TreeNode *parent, int &count) {
        if (!root) return 0;
        // 后序遍历
        int sameValueCountOfL = sameValueCount(root->left, root, count);
        int sameValueCountOfR = sameValueCount(root->right, root, count);
        int path = sameValueCountOfL + sameValueCountOfR; // 路径长度不算自己
        if (path > count)
            count = path;
        // 1. 当前节点与父节点不同; 路径断开了; 返回0
        if (!parent || root->val != parent->val)
            return 0;
        // 2. 当前节点与父节点相同; 路径连续的; 返回左右子树较大路径+1(自己); 
        return max(sameValueCountOfL, sameValueCountOfR)+1;
    }
public:
    int longestUnivaluePath(TreeNode* root) {
        if (!root) return 0;
        int count = 0;
        sameValueCount(root, NULL, count);
        return count;
    }
};