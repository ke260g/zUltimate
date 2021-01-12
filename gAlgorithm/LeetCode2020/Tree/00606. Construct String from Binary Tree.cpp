// Input: Binary tree: [1,2,3,4]
//        1
//      /   \
//     2     3
//    /    
//   4     
// Output: "1(2(4))(3)"


// Input: Binary tree: [1,2,3,null,4]
//        1
//      /   \
//     2     3
//      \  
//       4 

// Output: "1(2()(4))(3)"

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/construct-string-from-binary-tree
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    string tree2str(TreeNode* t) {
        // 1. 节点为空返回 ""
        // 2. 叶节点只返回值
        // 3. 只有左子树: 那么只递归左子树
        // 4. 右子树非空: 不管左子树是否为空; 都要递归
        if (!t) return "";
        if (!t->left && !t->right)
            return to_string(t->val);
        if (!t->right)
            return to_string(t->val) + "(" + tree2str(t->left) + ")";
        return to_string(t->val) + "(" + tree2str(t->left) + ")" + "(" + tree2str(t->right) + ")";
    }
};