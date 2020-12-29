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
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        // 1. 空节点返回空
        // 2. 两个链表; 一个表示当前层正在遍历的节点; 一个表示下一层将要遍历的节点
        // 3. 一个数组；用来存储当前层的结果
        // 4. 一个标记: 指示正方向 和 反方向
        // 5. 遍历当前层的每个节点:
        // 5.1 正方向
        // 5.1.1 当前层 链表头出列
        // 5.1.2 左节点非空 尾插法入列 下一层
        // 5.1.3 右节点非空 尾插法入列 下一层 (先左再右)
        // 5.2 反方向
        // 5.2.1 当前层 链表尾出列
        // 5.2.2 右节点非空 头插法入列 下一层
        // 5.2.3 右节点非空 头插法入列 下一层 (先右再左)
        // 5.3 当前层数值保存到结果数组中
        // 5.4 一层遍历完后
        // 5.4.1 遍历下一层swap
        // 5.4.2 并提交当前层数组; 
        // 5.4.3 清空数组
        // 5.4.4 切换方向
        vector<vector<int>> result;
        if (!root) return result;
        list<TreeNode *> curr_layer, next_layer;
        bool reverse_direction = false;
        vector<int> line;
        curr_layer.push_front(root);
        while(curr_layer.size()) {
            TreeNode * node = NULL;
            if (reverse_direction) {
                node = curr_layer.back();
                curr_layer.pop_back();
                if (node->right)
                    next_layer.push_front(node->right);
                if (node->left)
                    next_layer.push_front(node->left);
            } else {
                node = curr_layer.front();
                curr_layer.pop_front();
                if (node->left)
                    next_layer.push_back(node->left);
                if (node->right)
                    next_layer.push_back(node->right);
            }

            line.push_back(node->val);
            if (curr_layer.empty()) {
                result.push_back(line);
                line.clear();
                swap(curr_layer, next_layer);
                reverse_direction = !reverse_direction;
            }
        }
        return result;
    }
};