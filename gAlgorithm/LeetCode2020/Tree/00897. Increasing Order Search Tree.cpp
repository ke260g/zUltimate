// 给你一个树，请你 按中序遍历 重新排列树，使树中最左边的结点现在是树的根，
// 并且每个结点没有左子结点，只有一个右子结点。


// 输入：[5,3,6,2,4,null,8,1,null,null,null,7,9]

//        5
//       / \
//     3    6
//    / \    \
//   2   4    8
//  /        / \ 
// 1        7   9

// 输出：[1,null,2,null,3,null,4,null,5,null,6,null,7,null,8,null,9]

//  1
//   \
//    2
//     \
//      3
//       \
//        4
//         \
//          5
//           \
//            6
//             \
//              7
//               \
//                8
//                 \
//                  9  

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/increasing-order-search-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。


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
    void increasingBSTEx(TreeNode* root, TreeNode **iter, TreeNode **res) {
        // 中序遍历; 缓存结果res指针和res的步进指针
        if (!root) return;
        increasingBSTEx(root->left, iter, res);
        if (*iter) {
            (*iter)->right = new TreeNode(root->val);
            *iter = (*iter)->right;
        } else {
            *iter = new TreeNode(root->val);
            *res = *iter;
        }
        increasingBSTEx(root->right, iter, res);
    }
    TreeNode* increasingBST(TreeNode* root) {
        TreeNode *iter = NULL, *res = NULL;
        increasingBSTEx(root, &iter, &res);
        return res;
    }
};