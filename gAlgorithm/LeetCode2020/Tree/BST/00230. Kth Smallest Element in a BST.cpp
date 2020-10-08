// Given a binary search tree, write a function kthSmallest to find the kth smallest element in it.

// Input: root = [3,1,4,null,2], k = 1
//    3
//   / \
//  1   4
//   \
//    2
// Output: 1

// Input: root = [5,3,6,2,4,null,null,1], k = 3
//        5
//       / \
//      3   6
//     / \
//    2   4
//   /
//  1
// Output: 3

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/kth-smallest-element-in-a-bst
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 类似 LCOF 22. Return kth Node in a Linked List.cpp
// 需要递归栈回溯得到节点索引值
// 1. 中序遍历; 传递索引值 (最小节点索引是1)
// 2. 如果当前节点索引大于 k; 则返回 左子树求得返回值
// 3. 如果当前节点索引等于 k; 当前节点就是目标节点; 然回当前节点的值(不再需要 遍历右子树)
// 4. 如果当前节点索引小于 k；递归右子树; 然回右子树地值
class Solution {
public:
    int kthSmallestEx(TreeNode* root, int k, int &idx) {
        if (!root) return 0;
        int kInLeft = kthSmallestEx(root->left, k, idx);
        idx++;
        if (idx > k)
            return kInLeft;
        if (idx == k)
            return root->val;
        int kInRight = kthSmallestEx(root->right, k, idx);
        return kInRight;
    }
    int kthSmallest(TreeNode* root, int k) {
        int idx = 0; // 最小节点数的索引是1
        return kthSmallestEx(root, k, idx);
    }
};