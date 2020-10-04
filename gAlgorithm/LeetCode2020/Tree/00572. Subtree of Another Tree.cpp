// 给定两个非空二叉树 s 和 t，检验 s 中是否包含和 t 具有相同结构和节点值的子树。
// s 的一个子树包括 s 的一个节点和这个节点的所有子孙。s 也可以看做它自身的一棵子树。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/subtree-of-another-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// s:
//      3
//     / \
//    4   5
//   / \
//  1   2

//  t:
//     4 
//   / \
//  1   2

// Return true,

// 判断 t 是否是 s的子树
// 1. 两棵树都是空; 符合条件
// 2. 任一棵树非空; 不符合
// 3.1 要么 t 与 s 完全相同
// 3.2 要么 t 是 s 左子树的子树
// 3.3 要么 t 是 s 右子树的子树

// 判定相同树 00100. Same Tree
// 1. 两棵树都是空; 符合条件
// 2. 任一棵树非空; 不符合
// 3. 两树当前节点相等 且 两树的左右子树分别相同

class Solution {
public:
    bool isSameTree(TreeNode* x, TreeNode *y) {
        if (!x && !y) return true;
        if (!x || !y) return false;
        return x->val == y->val && isSameTree(x->left, y->left) && isSameTree(x->right, y->right);
    }
    bool isSubtree(TreeNode* s, TreeNode* t) {
        if (!s && !t) return true;
        if (!s || !t) return false;
        return isSameTree(s, t) || isSubtree(s->left, t) || isSubtree(s->right, t);
    }
};