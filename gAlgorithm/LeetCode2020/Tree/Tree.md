## 递归式 (根节点 且 左右子树)
00100. Same Tree
00111. Minimum Depth of Binary Tree
00112. Path Sum                     给定路径和 判断根结点到叶节点是否存在 等于该和的路径
00226. Invert Binary Tree           先翻转左右子树; 再翻转当前节点的左右节点 std::swap
00404. Sum of Left Leaves           求所有左叶节点的和; 提前判断左节点是否为叶
00559. Maximum Depth of N-ary Tree  先求得子树中的最大深度; 再向上返回
00572. Subtree of Another Tree      当前节点完全相同 or 左右子树其中之一包含 目标子树target
00617. Merge Two Binary Trees
00669. Trim a Binary Search Tree    先trim调左右子树; 再trim掉当前节点
00687. Longest Univalue Path        单值最长路径; 先计算左右子树各自的最长路径; 再把过当前节点的路径长度返回
00872. Leaf-Similar Trees           后序遍历得到叶子序列
00965. Univalued Binary Tree        单值二叉树

00671. Second Minimum Node In a Binary Tree 先在左右子树中找; 再检查当前节点是否满足条件 (这个不能用 INT_MAX)

### 递归左右子树时 返回两种信息
00110. Balanced Binary Tree         先根据判定左右子树; 再根据高度判断当前节点
    + 是否平衡 & 左右子树的高度
00543. Diameter of Binary Tree      树的直径长度; 先求左右子树的直径 和 高度; 再求当前节点的直径
    + 返回高度 & 根据左右子树高度求直径
00563. Binary Tree Tilt             坡度
    + 返回树的节点数值和 & 根据左右子树的节点数值和求坡度

## 层次遍历
00102. Binary Tree Level Order Traversal
00103. Binary Tree Zigzag Level Order Traversal 层次遍历加上方向
00107. Binary Tree Level Order Traversal II     层次遍历后 层次结果反转
00101. Symmetric Tree                           对称树的判断; 本质是对左右子树进行 ZigzagLevel 遍历
00637. Average of Levels in Binary Tree
00993. Cousins in Binary Tree                   记录 prev 层队列

## 简单遍历
00589. N-ary Tree Preorder Traversal
00590. N-ary Tree Postorder Traversal

## BST 问题
1. BST 多数中序遍历
```c++
if (*prev) {
    do_task
}
*prev = root; // 更新*prev节点
```
### 根据BST 值的分布特性; 带方向性地遍历
00235. Lowest Common Ancestor of a Binary Search Tree
00938. Range Sum of BST
00700. Search in a Binary Search Tree

### 缓存前置节点
00501. Find Mode in Binary Search Tree    中序遍历找众树
00530. Minimum Absolute Difference in BST 中序遍历求节点间的最小差的绝对值
00783. Minimum Distance Between BST Nodes 中序遍历求节点间的最小差