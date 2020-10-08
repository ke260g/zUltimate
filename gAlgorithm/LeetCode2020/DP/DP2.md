[TOC]
参考 @FennelDumplings 和 @jinlinpang 同学的意见; 对DP问题进行分类

# 01. 线性
## 单序列
00300. Longest Increasing Subsequence 最长递增子序列
00053. Maximum Subarray 最大子数列
00152. Maximum Product Subarray  乘积最大的子数列
00628. Maximum Product of Three Numbers 乘积最大的3个数

## 双序列
01035. Uncrossed Lines
01143. Longest Common Subsequence
00718. Maximum Length of Repeated Subarray

## 经典问题
00120. Triangle 三角形的最小路径和
00887. Super Egg Drop 鸡蛋掉落
00354. Russian Doll Envelopes 俄罗斯套娃信封

## 股票问题 (本质是单列)
00121. 买股票最佳时机
00122. 买股票最佳时机 II
00123. 买股票最佳时机 III
00188. 买股票最佳时机 IV
00309. 买卖股票地最佳时机包含冷冻期
00714. 买卖股票地最佳时机包含手续费

## 字串匹配 (本质是单列)
00072. 编辑距离
00044. 通配符匹配
00010. 正则式匹配


# 02. 区间
00516. Longest Palindromic Subsequence
00730. Count Different Palindromic Subsequences
01039. Minimum Score Triangulation of Polygon
00664. Strange Printer
00312. Burst Balloons

# 03. 背包类
00416. Partition Equal Subset Sum (01背包::恰好取到背包容量)
00494. Target Sum (01背包::求方案数)
00322. Coin Change (完全背包)
00518. Coin Change II (完全背包::求方案数)
00474. Ones and Zeroes (二维背包)

# 04. 树形
00124. Binary Tree Maximum Path Sum  (二叉树中的最大路径和)
01245. Tree Diameter           (邻接表上的树形DP)
00543. Diameter of Binary Tree (二叉树直径)
00333. Largest BST Subtree     (最大BST子树)

# 05. 数位
00233. Number of Digit One    (数字1的个数)
00902. Numbers At Most N Given Digit Set (最大为N的数字组合)
01015. Smallest Integer Divisible by K (可被K整除的最小整数)

# 06. 计数 (先用组合数学方法推导出组合数; 再写dp求组合数的算法)
00062. Unique Paths  (不同的路径)
00063. Unique Paths II  
00096. Unique Binary Search Trees  (卡特兰数)
01259. Handshakes That Don't Cross (卢卡斯定理求大组合数模质数)

# 07. 递推 (递推型本身也是单列; 一种特殊的单列)
00070. Climbing Stairs
00509. Fibonacci Number
00957. Prison Cells After N Days (第N天后的牢房)
01137. N-th Tribonacci Number  (第N个泰波那数列)
00198. House Robber
    + 递推公式 `f(n) = max(f(n-2), f(n-3)) + nums[n]`
    + 结果判定 `max(f(size-1), f(size-2))`
00213. House Robber II
    + 拆分3个区间使用求得原始问题的价值
    + `[0, size-2]` / `[1, size-1]` / `[1, size-2]`
    + 然后求 三者最大值
00337. House Robber III 是树型

# 08. 状态压缩 (递退过程中发现 状态可压缩)
00464. Can I Win
00526. Beautiful Arrangement   (完美排列)
00935. Knight Dialer           (骑士拨号)
01025. Divisor Game            (除法游戏)
01349. Maximum Students Taking Exam (参加考试的最大学生数)


# 09. 概率
00808. Soup Servings (分汤)
00837. New 21 Game (新21点游戏)

# 10. 博弈
00292. Nim Game
00293. Flip Game
00294. Flip Game II
00877. Stone Game
01140. Stone Game II
00348. Design Tic Tac Toe      判定井字游戏的正负
00794. Valid Tic Tac Toe State 有效的井字游戏
01275. Find Winner on a Tic Tac Toe Game 找出井字棋的获胜者

## 11. 记忆化搜索
00329. Longest Increasing Path in a Matrix
00576. Out of Boundary Paths