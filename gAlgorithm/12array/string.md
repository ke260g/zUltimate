### Q: 逆波兰表达式
+ 双栈实现/单栈实现/数实现

## Q: LCS (LongestCommonSequence 最长公共子序列 可不连续) (DP算法)

## Q: LIS (LongestIncreaseSequence) 最长递增子列

## Q: 字符串编辑距离

## Q: 字符串旋转k位
+ 三次拷贝算法: 时间O(n) 空间O(k)
+ 三次反转算法: 时间O(n) 空间O(1)  (X^r Y^r)^r = Y X

## Q: 全排列 permutation
+ 递归法: 排列后回溯初始数列 不断交换后递归排列
    + 用字典标记 相同的字符
+ 比较法: C++ next_permutation()
    + 先升序排列所有字符
    + 找下一个"稍微"比当前大的 字符串
    + 直到 所有字符 降序排列

## Q: 找子字符串 KMP算法
+ 时间O(m + n) 空间O(m)
+ 构造next数组, 每一项表示`s[0] ~ s[i-1]`中 头尾相同子串的长度
    + 利用递推关系求
    + next数组的递归原理
    + 递归到0时的出口处理
+ next数组的变种: 递归地 `next[i] = next[next[i]]`
+ Q: 循环序列的 判定 以及 最小周期
    + `if len  % len(s) - next[s - 1] - 1 == 0`
    + 如果能整除 说明是循环的
    + 最小周期为 `len(s) - next[s - 1] - 1`

## Q: 找子字符串 BM算法 BladeMaster
+ pattern 从后往前比对
+ 遇到text中的"坏字符" 右移pattern找到对应的坏字符
  重新 pattern 从后往前比对
+ 初始化时 hash pattern 减低坏字符 查找耗时

## Q: 找回文子串 manacher算法
1. 构造gap序列 使得查找的序列长度必定为 奇数
2. 利用对称性 `r[i] = min(r[2*iM - i], iM - i)`
    + `r[i]` 为每个index中 前后相同的字符个数 最小为1
    + 无法利用对称性的就 暴力解
3. `r[i]`为偶数 对应的字符必然是gap的填充字符