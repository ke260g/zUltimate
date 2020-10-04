// Given a binary search tree with non-negative values, 
// find the minimum absolute difference between values of any two nodes.

// Input:
//    1
//     \
//      3
//     /
//    2

// Output:
// 1

// Explanation:
// The minimum absolute difference is 1, which is the difference between 2 and 1 (or between 2 and 3).

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/minimum-absolute-difference-in-bst
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
    // 根据BST的特性遍历; 上一个遍历的节点值 一定小于等于 当前节点值
    // 1. 根据 当前节点于上一个节点的差值 与 缓存的结果比较
    // 2. 如果 差值比较少; 那就更新缓存结果
    // 3. *prev 指针
    void getMinimumDifferenceEx(TreeNode *root, TreeNode **prev, int *res) {
        if (!root) return;
        getMinimumDifferenceEx(root->left, prev, res);
        if (*prev) {
            int diff = root->val - (*prev)->val;
            if (diff < *res)
                *res = diff;
        }
        *prev = root;
        getMinimumDifferenceEx(root->right, prev, res);
    }
public:
    
    int getMinimumDifference(TreeNode* root) {
        int res = INT_MAX;
        TreeNode *prev = NULL;
        getMinimumDifferenceEx(root, &prev, &res);
        return res;
    }
};