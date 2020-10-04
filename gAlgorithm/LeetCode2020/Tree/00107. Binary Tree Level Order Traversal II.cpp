// Input:
//     3
//    / \
//   9  20
//     /  \
//    15   7
//
// Output: 
// [
//   [15,7],
//   [9,20],
//   [3]
// ]

// 层次遍历; 但是反转层次的结果
// 经典层次遍历操作; 最后把层次遍历的 vector<vector<int>> 倒转一次即可
// 参考 00102. Binary Tree Level Order Traversal 题

class Solution {
public:
    vector<vector<int>> levelOrderBottom(TreeNode* root) {
        vector<vector<int>> res;
        if (!root) return res;
        queue<TreeNode *> curr_layer;
        queue<TreeNode *> next_layer;
        vector<int> layer;
        curr_layer.push(root);
        while (!curr_layer.empty()) {
            TreeNode *n = curr_layer.front();
            curr_layer.pop();
            layer.push_back(n->val);
            if (n->left) next_layer.push(n->left);
            if (n->right) next_layer.push(n->right);
            if (curr_layer.empty()) {
                swap(curr_layer, next_layer);
                res.push_back(layer);
                layer.clear();
            }
        }
        for (int i = 0; i < (int)res.size()/2; ++i) {
            swap(res[i], res[res.size()-1-i]);
        }
        return res;
    }
};