// 给定一个非空特殊的二叉树，每个节点都是正数，并且每个节点的子节点数量只能为 2 或 0。
// 如果一个节点有两个子节点的话，那么该节点的值等于两个子节点中较小的一个。
// 给出这样的一个二叉树，你需要输出所有节点中的第二小的值。如果第二小的值不存在的话，输出 -1 。

// 示例 1:
// 输入: 
//     2
//    / \
//   2   5
//      / \
//     5   7

// 输出: 5
// 说明: 最小的值是 2 ，第二小的值是 5 。

// 这一题有一个陷阱: 就是 INT_MAX 可以作为节点成员出现
// 如果简单地初始化 candicate(2, INT_MAX); 然后在遍历时把节点值插入到合适位置 会出错
// 所以: 正确的做法时 初始化 candicate 空数组; 在遍历过程中增加到尺寸2
//       然后在遍历时; 当发现candicate已经有两个成员时; 再把当前节点插入合适位置
class Solution {
public:
    void findSecondMinimumValueEx(TreeNode *root, vector<int> &candicate) {
        // 1. 后序遍历; 维护"候选者"数组(升序)
        // 2. 候选者数组即遍历过程中最小的两个值(不相同的) 
        // 2.1 候选者为空; 插入节点值即可
        // 2.2 候选者只有一个; 插入节点值后调整大小位置
        // 2.3 候选者已经有两个; 根据节点值插入到数组的合适位置
        if (!root) return;
        findSecondMinimumValueEx(root->left, candicate);
        findSecondMinimumValueEx(root->right, candicate);
        if (candicate.empty()) {
            candicate.push_back(root->val);
        } else if (candicate.size() == 1) {
            if (root->val != candicate[0]) {
                candicate.push_back(root->val);
                if (candicate[0] > candicate[1])
                    swap(candicate[0], candicate[1]);   
            }
        } else if (candicate.size() == 2) {
            if (root->val < candicate[0]) {
                candicate[1] = candicate[0];
                candicate[0] = root->val;
            } else if (candicate[0] < root->val &&
                       root->val < candicate[1]) {
                candicate[1] = root->val;
            }
        }
    }
    int findSecondMinimumValue(TreeNode* root) {
        // 不能直接地使用 candicate(2, INT_MAX); 
        // 因为 INT_MAX 可以作为树木的成员出现
        vector<int> candicate;
        findSecondMinimumValueEx(root, candicate);
        return candicate.size() == 2 ? candicate[1] : -1;
    }
};