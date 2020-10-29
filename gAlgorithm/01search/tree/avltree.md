[TOC]

# AVL 树 定义 (2个)
1. 任意节点的左右子树都是一颗 AVL 树
2. 任意节点的左子树和右子树的高度之差(平衡因子)的绝对值不超过1

# AVL 树 特征 (4个)
1. AVL 树 高度是 `O(log2n)`
2. 查找的时间复杂度`O(log2n)`
3. 插入的时间复杂度`O(log2n)`
4. 删除的时间复杂度`O(log2n)`

# AVL 树 其他
1. 不平衡时; 平衡因子的取值范围 -2, -1, 0, 1, 2

# AVL 规律
1. 失衡节点到叶子节点的高度小于等于4
2. 失衡节往不平衡的方向反向旋转
3. 插入节点到失衡节点的路径如果是一条直线(LL RR)，则只需对失衡节点进行反向旋转
4. 插入节点到失衡节点的路径如果是一条曲线(LR RL)，
   则需先对该路径上失衡节点的子节点(left / right)进行旋转, 变换为直线路径
   然后对失衡节点进行反向旋转
5. 失衡节点旋转后会成为它原来子树(left / right)中的一颗子树，
   而原来子树的子树(孙) 则会以失衡节点作为父亲

# 实现要点
## 1. 定义节点
1. 节点必须有指向父节点的指针
2. 每个节点必须维护左右子树的高度??
3. 每个节点带有  "平衡因子" = 左子树高度 - 右子树高度

## 2. 新增节点
```c++
void insert(struct node *root, int val) {
    // 步骤1: 二叉搜索树的方法插入节点
    if (val < root->val) { // 小于根节点则往左边插入
        if (root->left){
            insert(root->left, val);
        } else {
            root->left = new node(val);
            root->left->parent = root;
        }
    } else { // 往右边插入
        if (root->right)
            insert(root->right, val);
        else {
            root->right = new node(val);
            root->right->parent = root;
        }
    }
    // 步骤2: 计算深度 和 平衡因子
    calc_depth_and_balance(root);

    // 步骤3: 检查不平衡节点 然后调整
    unbalanced_node_detect_and_adjust(root);
}
```
1. 实际上; insert方法递归; 递归返回时从插入节点逐层往上 计算深度和平衡因子
    + 递归返回时即插入路径
2. 插入路径每一个节点都需要进行不平衡节点判定; 
    + 通过递归返回的方式; 实现原路返回

## 3. 删除节点

## 4. 平衡因子的计算方法
+ 拆分两个逻辑; 计算深度度 和 计算平衡因子
```c++
void calc_depth(struct node *root){
    int left_depth  = root->left  ? root->left->depth  : 0;
    int right_depth = root->right ? root->right->depth : 0;
    root->depth = max(depth_on_left, depth_on_right) + 1;
}

void calc_balance(struct node *root){
    int left_depth  = root->left  ? root->left->depth  : 0;
    int right_depth = root->right ? root->right->depth : 0;
    root->balance = left_depth - right_depth;
}
void calc_depth_and_balance(struct node *root) {
    calc_depth(root);
    calc_balance(root);
}
```

## 5. 失衡节点的判定和调整 (类型判定 和 旋转调整)  (LL RR LR RL)
```c++
void unbalanced_node_detect_and_adjust(struct node *root) {
    if (root->balance >= 2) { // LR型 和 LL 型
        if (root->left->balance == -1)  {   // LR型: 先左旋左节点; 变换为LL型
            left_rotate(root->left);
        }
        right_rotate(root);               // LL型: 右旋不平衡节点
    }
    if (root->balance <= -2) { // RL型 和 RR 型
        if (root->right->balance == 1) {
            right_rotate(root->right);    // RL型: 先右旋右节点; 变换为RR型
        }
        left_rotate(root);                // RR型: 左旋不平衡节点
    }
}
```

## 6. 旋转方法 (左旋 和 右旋)
```c++
/*
 parent                  parent
 /   \                   /   \
...  root              ...  child
     / \                     / \
    Ta  child             root Tb
       / \                / \
   grand  Tb             Ta grand
 */
void left_rotate(struct node *root) {
    struct node *parent = root->parent;
    struct node *child = root->left;  // 子
    if (!child) return; // 没法旋转
    struct node *grand = child->right; // 孙

    // 步骤1: 子变根
    child->parent = parent;
    if (parent != NULL) {
        if (parent->left == root)
            parent->left  = child;
        else // curr 是父节点的有节点
            parent->right = child;
    }

    // 步骤2: 根变子
    child->left = root;
    root->parent = r;

    // 步骤3: 孙以根为父
    root->right = grand;
    if (grand) grand->parent = root;

    // 步骤4: 重新计算 根节点 的深度和平衡因子
    calc_depth_and_balance(root);
    // 步骤5: 重新计算 右节点 的深度和平衡因子
    calc_depth_and_balance(child);
}

/*
      parent                  parent
      /   \                   /   \
    root  ...               child  ...
    / \                     / \
child  Ta                  Tb root
 / \                          /  \
Tb grand                   grand  Ta
旋转前 ==================> 旋转后
 */
void right_rotate(struct node *root) {
    struct node *parent = root->parent;
    struct node *child  = root->left;   // 子
    if (!child) return;   // 没法旋转
    struct node *grand  = child->right; // 孙

    // 步骤1: 子变根
    child->parent = parent;
    if (!parent) {
        if (parent->left == root)
            parent->left  = child;
        else
            parent->right = child;
    }
    // 步骤2: 根变子
    child->right = root;
    root->parent = child;

    // 步骤3: 孙以根为父
    root->left = grand;
    if (grand) grand->parent = root;

    // 步骤4: 重新计算 根节点 的深度和平衡因子
    calc_depth_and_balance(root);
    // 步骤5: 重新计算 子节点 的深度和平衡因子
    calc_depth_and_balance(child);
}
```
1. 注意; 旋转后; 传入参数根结点 所在的位置是会改变的
2. 步骤4 和 步骤5 的顺序不能颠倒
4. 步骤4 和 步骤5 仅用于AVL树; 如果红黑树 则是调整颜色
