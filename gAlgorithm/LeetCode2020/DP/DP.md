## 当前元素i 的结果有元素 i 之前的若干个元素结果确定
00070. Climbing Stairs          爬楼梯问题 f(x) = f(x-1) + f(x-2)
00198. House Robber             打劫问题   f(x) = value(x) + max(f(x-2), f(x-3))
00746. Min Cost Climbing Stairs 爬楼梯最小体力消耗问题 f(x) = cost(x) + min(f(x-1), f(x-2))
LCCI. 08.01.Three Steps Problem 爬楼梯问题II f(x) = f(x-1) + f(x-2) + f(x-3)
LCCI.17.16. The Masseuse        按摩师问题 f(x) = value(x) + max(f(x-2), f(x-3)) 等同打劫问题

## 遍历过程中 更新全局同一标记位 (元素i的结果 不由元素i之前的元素确定)
00053. Maximum Subarray   指示当前元素i 是否作为子数列成员or子数列的头
    1. f(i-1)  < 0;  a[i] 作为子数列的头
    2. f(i-1) >= 0;  a[i] 作为子数列的结尾
    3. sum(子数列)
00121. Best Time to Buy and Sell Stock 每天都可以卖出股票；每天可以用更低价格买入
    1. profit = max(a[i] - buyin, profit); 
    2. if (a[i] < buyin) buyin = a[i];
00628. Maximum Product of Three Numbers 遍历过程中维护最大的3个数 和 最小的2个数(负数)
    1. 把符合条件的 a[i] 插入的 `max3[3]` 里面
    2. 把符合条件的 a[i] 插入到 `min2[2]` 里面

## 奇偶问题
01025. Divisor Game            除数游戏