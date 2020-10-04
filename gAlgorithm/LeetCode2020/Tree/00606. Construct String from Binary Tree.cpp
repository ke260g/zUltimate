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
        // 1. 空节点不输出
        // 2. 叶节点只输出自己
        // 3. 左节点无论存在; 都需要输出()
        // 4. 输出非空右节点; 左右都要有()
        string res;
        // 本质是前序遍历
        if (!t) return res;
        res += to_string(t->val);
        if (!t->left && !t->right)
            return res;
        res += "(";
        if (t->left)
            res += tree2str(t->left);
        res += ")";

        if (t->right) {
            res += "(";
            res += tree2str(t->right);
            res += ")";
        }
        return res;
    }
};