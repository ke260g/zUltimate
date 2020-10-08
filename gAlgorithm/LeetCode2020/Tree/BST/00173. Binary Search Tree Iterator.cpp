// Implement an iterator over a binary search tree (BST). Your iterator will be initialized with the root node of a BST.
// Calling next() will return the next smallest number in the BST.
//
// BST: 
//    7
//  /   \
// 3    15
//     /  \
//    9   20
// BSTIterator iterator = new BSTIterator(root);
// iterator.next();    // return 3
// iterator.next();    // return 7
// iterator.hasNext(); // return true
// iterator.next();    // return 9
// iterator.hasNext(); // return true
// iterator.next();    // return 15
// iterator.hasNext(); // return true
// iterator.next();    // return 20
// iterator.hasNext(); // return false
//
// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/binary-search-tree-iterator
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 实现一个BST 的迭代器; next 方法 O(1) 复杂度
// 方法1(lowB 方法): 把输入地BST 变换为1维的
// 方法2(缓存递归法): 维护递归过程中的路径节点栈

// 路径节点栈:
//   从根节点到当前迭代器返回节点的路径; 不包括当前节点自己
// 初始化:
// 1. 找到最小节点start  left->left->...; 同时维护路径节点栈
// 2. 找到最大节点 stop  right->right->...
// 3. 当前节点设置为 nullptr
// 迭代next:
// 1. 当前节点为 nullptr 则设置为 start (首次开始)
// 2. 如果已经时最节点; 返回0; 错误规避
// 3. 如果当前节点有右节点; 则找到右子树中的最小节点; 同时维护路径节点栈
//    curr->right->left->left->left->... (找后继节点场景1)
// 4. 如果当前节点无右节点; 则从路径栈中找到首个作为 左孩的节点;
//    该节点的父节点(栈的再上一级) 即curr的后继
//    如下图: 若curr 为 D; 后继是A; 路径栈此时为 A-B-C
//    首个作为左孩的节点是B; B地父节点是A
//    A
//   /
//  B
//   \
//    C
//     \
//      D
//    如下图: 若curr 为 B; B 已经是左孩(无右节点); 则返回其父
//    A
//   /
//  B

// 自己实现的方法2 (提交记录 双90%)
class BSTIterator {
private:
    TreeNode *curr;
    TreeNode *stop;
    TreeNode *start;
    stack<TreeNode *> path;
public:
    BSTIterator(TreeNode* root) {
        // 开头 curr
        if (!root) start = nullptr;
        else if (!root->left) start = root;
        else {
            start = root;
            while (start->left) {
                path.push(start);
                start = start->left;
            }
        }
        // 结尾
        if (!root) stop = nullptr;
        else if (!root->right) stop = root;
        else {
            stop = root->right;
            while(stop->right)
                stop = stop->right;
        }
        curr = nullptr;
    }
    /** @return the next smallest number */
    int next() {
        if (curr == nullptr) {      // 开始
            curr = start;
        } else if (curr == stop) {  // 结束
            return 0;
        } else if (curr->right) {   // 找后继; 有右子树; 找右子树最小的
            path.push(curr); // 当前节点入栈; 路径栈不包含当前节点
            curr = curr->right;
            while (curr->left) {
                path.push(curr);
                curr = curr->left;
            }
        } else {                    // 找后继; 无右子树; 返回父母节点
            if (!path.empty()) {
                while (curr == path.top()->right) {
                    curr = path.top();
                    path.pop();
                }
                curr = path.top();
                path.pop();
            } else 
                return 0; // error; 理论上不该跑到这里
        }
        return curr->val;
    }
    /** @return whether we have a next smallest number */
    bool hasNext() {
        return curr != stop;
    }
};

/**
 * Your BSTIterator object will be instantiated and called as such:
 * BSTIterator* obj = new BSTIterator(root);
 * int param_1 = obj->next();
 * bool param_2 = obj->hasNext();
 */