[TOC]
https://zhuanlan.zhihu.com/p/79980618
# 描述
## 插入描述
1. 将新节点颜色颜色为红色; 根据二叉搜索树的规则插入
2. parent 为红色 且 uncle 为红色
    1. uncle 和 parent 的颜色变换为黑色
    2. grand的颜色变换为红色
    3. grand作为curr 继续调整颜色
2. parent 为红色 且 uncle 是黑色  // <-- 这里要旋转主要是因为叔为NULL时; 
    1. 根据 LL LR RR RL 型; 对 grand 作形同 AVL树的旋转
    2. 调整旋转节点的颜色 (旋转后的颜色调整不同于 步骤2的颜色调整)
3. 检查根节点，根节点为红色则染黑

```c++
/*
parent 和 uncle 都是红色; 不需要旋转
      grand(b)                             grand(r)
       |   |                                |   |
uncle(r)  parent(r)                  uncle(b)  parent(b)
    |      |     |                       |      |     |
...(b)  curr(r) brother(b)           ...(b)  curr(r) brother(b)
  颜色调整前     ======================>  颜色调整后

parent 是红色 而 uncle 都是黑色; 的4种类型
       grand(b)                         parent(b)
      /       \                        /        \
uncle(b)     parent(r)             grand(r)      curr(r)
    |        /       \            /       \
 ...(b)  brother(b)  curr(r)   uncle(b) brother(b)
        LL型     ========================> 旋转调整后

      grand(b)
       /   \
uncle(b)  parent(r)
    /      /
 ...(b)  curr(r)
        LR型
 */

 // LL型 右旋后; parent 变换为黑色; grand变换为红色
 // LR型 先左旋(不调整颜色); 再右旋转, curr变换为黑色; grand变换为红色
 // RR型 左旋后; parent 变换为黑色; grand变换为红色
 // RL型 先右旋(不调整颜色); 再左旋转, curr变换为黑色; grand变换为红色 
```

## 删除描述
1. 根据二叉搜索树的规则删除


# 实现
## get_color / get_brother / get_uncle
```c++
int get_color(struct node *root) {
    return root ? root->color : black; // 空节点为黑色
}
struct node *get_brother(struct node *root) {
    if (!root || !root->parent) return root;
    return root == root->parent->left ? root->parent->right : root->parent->left;
}
struct node *get_uncle(struct node *root) {
    return root ? get_brother(root->parent) : root;
}
```