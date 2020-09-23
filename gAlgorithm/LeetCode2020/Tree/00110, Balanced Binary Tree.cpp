

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
    bool result = true;
    int calc_tree_depth(TreeNode *root) {
        if (!result) return result; // 提前结束递归 (超级关键；递归算法的提前结束机制)
        // 1. 空树是平衡的
        // 2. 先判断左子树
        // 3. 再判断右子树
        // 4. 根据高度差判断
        // +  递归返回高度差
        // 1. 空树是平衡的
        if (!root)
            return 0;
        int left_depth = 0, right_depth = 0;
        // 2. 先判断左子树
        left_depth = calc_tree_depth(root->left);
        if (!result) return false;

        // 3. 再判断右子树
        right_depth = calc_tree_depth(root->right);
        if (!result) return false;

        // 4. 根据高度差判断
        int delta_depth = left_depth - right_depth;
        if (abs(left_depth - right_depth) > 1) {
            result = false;
            return false;
        }
        // +  递归返回高度差
        return max(left_depth, right_depth) + 1;
    }
    bool isBalanced(TreeNode* root) {
        calc_tree_depth(root);
        return result;
    }
};

// version1: 本质是前序遍历; 效率低, 这样子的复杂度是 O(n!), 遍历子树时重复计算了
class Solution {
public:
    int calc_tree_depth(struct node *root) {
        if (root) return 0;
        int left_depth  = calc_btree_depth(root->left);
        int right_depth = calc_btree_depth(root->right);
        return max(left_depth, right_depth) + 1;
    }
    bool isBalanced(TreeNode* root) {
        // 1. 空的树也是一棵平衡二叉树
        // 2. 根据左右子树的高度差判定 (大于1, 则不是平衡)
        // 3. 先判定左子树
        // 4. 再判定右子树 (因为存在 两子树都不是平衡二叉树, 但高度差符合条件相等的情况)
        if (!root) return true; 
        int left_depth  = calc_tree_depth(root->left);
        int right_depth = calc_tree_depth(root->right);
        int delta_depth = left_depth - right_depth;
        if (abs(left_depth - right_depth) > 1)
            return false;
        return isBalanced(root->left) && isBalanced(root->right);
    }
}

// version2: 本质是后序遍历; 稍微有提高
class Solution {
public:
    bool __isBalanced(TreeNode *root, int *depth) {
        // 1. 空树也是平衡二叉树
        // 2. 先判定左子树
        // 3. 再判断右子树
        // 4. 根据左右子树的高度差判定
        int left_depth = 0;
        int right_depth = 0;
        
        if (!root) {
            if (depth) *depth = 0;
            return true;
        }

        if (!__isBalanced(root->left, &left_depth) ||
            !__isBalanced(root->right, &right_depth))
            return false
        
        // 4. 根据左右子树的高度差判定
        if (abs(left_depth - right_depth) > 1)
            return false;

        // 5. 返回高度 (只有平衡时候; 高度计算有意义; 否则不计算高度)
        if (depth) *depth = max(left_depth, right_depth) + 1;
        return true;
    }
    bool isBalanced(TreeNode* root) {
        return __isBalanced(root);
    }
}

///// 完全二叉树检测
bool check_complete_btree(struct node *root) {
    // 用两个队列 广度遍历二叉树
    // 一个队列当前当前层数; 另一个队列记录下一个层数
    // 遍历每层节点时; 判断连续两个节点是否指向同一个的父节点
    queue<struct node *> curr_layer_queue;
    queue<struct node *> next_layer_queue;
    struct node *common_parent = NULL;

    if (!root) return false;
    curr_layer_queue.push(root);
    while (curr_layer_queue.size()) { // 层级遍历
        struct node * n = curr_layer_queue.front();
        curr_layer_queue.pop();

        if (n->parent) { // 非根节点
            if (common_parent) {
                if (n->parent == common_parent) // 当前与上一个节点有共同父节点
                    common_parent = NULL;
                else // 没有共同父节点; 则不是完全二叉树
                    return false;
            } else {
                common_parent = n->parent;
            }
        }
    
        // 层级遍历
        if (n->left)   next_layer_queue.push(n->left);
        if (n->right)  next_layer_queue.push(n->right);

        if (!curr_layer_queue.size()) // 层级遍历; 当前层结束；进入下一层
            swap(curr_layer_queue, next_layer_queue);
    }

    return true;
}