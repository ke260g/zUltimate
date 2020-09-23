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
    vector<vector<int>> levelOrder(TreeNode* root) {
        // 1. 空节点返回空
        // 2. 两个队列; 一个表示当前层正在遍历的节点; 一个表示下一层将要遍历的节点
        // 3. 一个数组；用来存储当前层的结果
        // 4. 遍历当前层的每个节点:
        // 4.1 当前节点出列当前层
        // 4.2 节点值存储到数组中
        // 4.3 左节点非空入列 下一层
        // 4.4 右节点非常空入列 下一层
        // 4.5 一层遍历完后; 遍历下一层swap; 并提交当前层数组; 清空数组
        vector<vector<int>> result;
        if (!root) return result;
        queue<TreeNode *> curr_layer, next_layer;
        vector<int> line;
        curr_layer.push(root);
        while(curr_layer.size()) {
            TreeNode *node = curr_layer.front();
            curr_layer.pop();
            line.push_back(node->val);
            if (node->left)
                next_layer.push(node->left);
            if (node->right)
                next_layer.push(node->right);
            
            if (!curr_layer.size()) {
                result.push_back(line);
                line.clear();
                swap(curr_layer, next_layer);
            }
        }
        return result;
    }
};