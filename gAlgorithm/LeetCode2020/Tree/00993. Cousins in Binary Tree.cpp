// Example 1:
//     1
//    / \
//   2   3
//  /
// 4
// Input: root = [1,2,3,4], x = 4, y = 3
// Output: false

// 给定没有重复值的二叉树; 给定两个节点的值
// 如果两个节点的深度相同; 且父母不同; 那么两个节点就是 堂兄弟姐妹 节点

class Solution {
public:
    bool isCousins(TreeNode* root, int x, int y) {
        // 1. 层次遍历求得 x, y 的各自深度;
        //    当深度相同时; 如何判定他们是否有共同的父节点?
        // 2. 缓存3层队列; prev, curr, next,
        // 3. x, y节点先后后被找到后; 判断x, y 的深度是否相等;
        // 4. 如果不等; 则不相等 
        // 5. 如果相等; 则遍历prev 层队列; 判断能否找到他们的共同父亲
        list<TreeNode *> prev, curr, next, curr_r;
        int depthOfX = -1, depthOfY = -1;
        int depth = 0;
        if (!root) return false;
        curr.push_back(root);
        while (!curr.empty()) {
            // 当前层节点出列
            TreeNode *node = curr.front();
            curr.pop_front();
            curr_r.push_back(node);

            // 记录找到的深度; x,y都找到了退出循环
            if (node->val == x)
                depthOfX = depth;
            if (node->val == y)
                depthOfY = depth;
            if (depthOfX >= 0 && depthOfY >= 0)
                break;
            
            // 还没找到就继续层次遍历
            if (node->left) next.push_back(node->left);
            if (node->right) next.push_back(node->right);
            
            // 继续下一层
            if (curr.empty()) {
                prev.clear();
                swap(prev, curr_r);
                
                swap(curr, next);
                depth++;
            }
        }
        // 没找到
        if (depthOfX < 0 || depthOfY < 0) return false;
        // 深度不同
        if (depthOfX != depthOfY) return false;
        // 找共同父亲
        while (!prev.empty()) {
            TreeNode * node = prev.front();
            prev.pop_front();
            if (!node->left || !node->right)
                continue; // 单枝父亲不能时共同父亲
            if (node->left->val == x && node->right->val == y)
                return false; // 共同父亲
            if (node->right->val == x && node->left->val == y)
                return false; // 共同父亲
        }
        return true;
    }    
};