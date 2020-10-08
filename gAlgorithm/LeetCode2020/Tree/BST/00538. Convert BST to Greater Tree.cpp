// 给出二叉 搜索 树的根节点，该树的节点值各不相同，请你将其转换为累加树（Greater Sum Tree），
// 使每个节点 node 的新值等于原树中大于或等于 node.val 的值之和。

// 提醒一下，二叉搜索树满足下列约束条件：
//     节点的左子树仅包含键 小于 节点键的节点。
//     节点的右子树仅包含键 大于 节点键的节点。
//     左右子树也必须是二叉搜索树。

// 输入：root = [0,null,1]
// 输出：[1,null,1]

// 输入：root = [1,0,2]
// 输出：[3,3,2]

// 输入：root = [3,2,4,1]
// 输出：[7,9,4,10]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/kth-smallest-element-in-a-bst
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 按照题意; 如果BST 的数值序列为 { a, b, c, d, e, } 升序
// 则变换后: d += e; c += d + e; b += c + d + e;
// 因此: 
// 1. 采用 右-根-左 遍历次序；
// 2. 传递BST后继节点
// 3. 当前节点的值 += 已变换的后继结点的值

class Solution {
public:
    TreeNode* convertBSTEx(TreeNode* root, TreeNode **next) {
        if (!root) return nullptr;
        root->right = convertBSTEx(root->right, next);
        if (*next)
            root->val += (*next)->val;
        *next = root;
        root->left = convertBSTEx(root->left, next);
        return root;
    }

    TreeNode* convertBST(TreeNode* root) {
        TreeNode *next = nullptr;
        return convertBSTEx(root, &next);        
    }
};