## Q: LCA (Lowest Common Ancestor 最近公共组祖先)
+ 给定左节点 与 右节点 求在二叉树中的最近公共祖先 (没有父指针)
+ 

## Q: 匹配 ()[]{} 成对匹配
+ 小括号: Parentheses bracket
+ 中括号: bracket 
+ 大括号：braces
+ 方法:
    1. 遇到([{ 入栈 
    2. 遇到)]} 判断栈顶是否匹配 若能匹配则出栈

## Q: 最长括号匹配 (仅包含 左右括号 的字符串)
+ ()() : 4
+ (())() : 4
+ tricks: 入栈的是 index, 表明左括号的位置

## Q: 已知 中序表达式(字符串) 求 逆波兰表达式 RPN
+ 如 `a+(b-c)*d` 输出 a(bc-)d*+
1. 两个栈 abcd 和 `+(-)*`

## Q: 已知 逆波兰表达式(字符串) 求 值
1. 遇到操作数入栈
2. 遇到操作符, 出栈两个操作数求值后入栈