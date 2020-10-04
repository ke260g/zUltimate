// 计算二叉树的每层平均值

// Input:
//     3
//    / \
//   9  20
//     /  \
//    15   7
// Output: [3, 14.5, 11]
// Explanation:
//     The average value of nodes on level 0 is 3,  
//     on level 1 is 14.5, 
//     and on level 2 is 11. 
//     Hence return [3, 14.5, 11].

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/average-of-levels-in-binary-tree
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
    vector<double> averageOfLevels(TreeNode* root) {
        // 层次遍历; 计算每层的节点平均值
        vector<double> res;
        if (!root) return res;
        list<TreeNode *> curr_layer;
        list<TreeNode *> next_layer;
        curr_layer.push_back(root);
        double resOfLayer = 0.0;
        int countOfLayer = 0;
        while (!curr_layer.empty()) {
            TreeNode *node = curr_layer.front();
            curr_layer.pop_front();
            resOfLayer += node->val;
            countOfLayer++;
            if (node->left) next_layer.push_back(node->left);
            if (node->right) next_layer.push_back(node->right);
            if (curr_layer.empty()) {
                res.push_back(resOfLayer / countOfLayer);
                swap(curr_layer, next_layer);
                countOfLayer = 0;
                resOfLayer = 0.0;
            }
        }
        return res;
    }
};