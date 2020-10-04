// 找到BST的众数

// 官网的答案;
// 根据BST中序遍历的特性; 用前置节点指针 和 int 的count
// 比起暴力求解; 节省了count的空间 和 hash的运算
class Solution {
public:
    void findModeEx(TreeNode *root, vector<int> &res, int &count, int &max, TreeNode **prev) {
        // BST 的中序遍历; 遍历元素一定是升序
        // 1. 空树直接返回
        // 2. 先遍历左子树
        // 3. 当前节点处理
        // 3.1 极左点; *prev 为空; 则赋值; 当前相同元素计数器count = 1;
        // 3.2 非极左; 上一节点值 不等于 当前节点值； 计数器count = 1;
        // 3.3 非极左; 上一节点值 等于   当前节点值; 计数器count++;
        // 4. 根据count 判断当前节点值是否是众数
        // 4.1 count  > max ( 更新max; 置空res; val作为res )
        // 4.2 count == max ( val 加入 res 中)
        if (!root) return;
        findModeEx(root->left, res, count, max, prev);
        if (!(*prev) || (*prev)->val != root->val) {
            *prev = root;
            count = 1;
        } else {
            count++;
        }
        if (count > max) {
            res.clear();
            max = count;
            res.push_back(root->val);
        } else if (count == max) {
            res.push_back(root->val);
        }
        findModeEx(root->right, res, count, max, prev);
    }
    vector<int> findMode(TreeNode* root) {
        int max = 0;
        vector<int> res;
        int count = 0;
        TreeNode *prev = NULL;
        findModeEx(root, res, count, max, &prev);
        return res;
    }
};


// 自己想的暴力求法; 自己的求法本质上没有用到 BST的特性
// 1. 缓存当前 max 和 count
// 2. 中序遍历
// 3. 遍历当前节点时; count[val]++
// 4. 如果更新后 count[val] 大于 max; 说明出现新的众数;
//    清空res结果数组;
//    val加入res即可
//    更新max
// 5. 如果更新后; count[val] 等于 max; 说明由相同次数的众数出现
//    val加入res即可
/**
 * @max: 相同数值的最大出现次数
 * @count: 已遍历数字的出现次数
 */
class Solution {
public:
    void findModeEx(TreeNode *root, vector<int> &res, map<int, int> &count, int &max) {
        // BST 中序遍历;
        if (!root) return;
        findModeEx(root->left, res, count, max);
        count[root->val]++;
        if (count[root->val] > max) {
            res.clear();
            res.push_back(root->val);
            max = count[root->val];
        } else if (count[root->val] == max) {
            res.push_back(root->val);
        }
        findModeEx(root->right, res, count, max);
    }
    vector<int> findMode(TreeNode* root) {
        int max = 0;
        vector<int> res;
        map<int, int> count;
        findModeEx(root, res, count, max);
        return res;
    }
};