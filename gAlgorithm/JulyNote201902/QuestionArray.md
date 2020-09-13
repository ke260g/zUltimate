# 数组问题
+ 问题非常难 甚至是NP难解
+ 从特殊到一般的分析思路

### Q: 最大连续子数组 (连续子数组 的和为最大值)
+ 穷据法 T(n) = O(n^3)
+ 分治法 T(n) = O(n*log2n)
    + 对于任意i项 最大子数组 包住/在左边/在右边
+ 分析法 T(n) = O(n)
    + 求 n项前缀和 p[]; p的头补一个0
    + 记录当前最小的`p[iMin]` 最大的差值`p_max`
    + 对于每个 `p[i] - p[iMin]` 与 `p_max`比较
+ 动态规划 T(n) = O(n)
    + 对于任意i项
      要么是 前一个 最大子数组的尾
      要么是 新一个 最大子数组的头

### Q: 零子数组 (连续子数组 的和的绝对值 最小)
+ 求 n项前缀和
+ 排序 n项前缀和, 

### Q: 已知旋转数组 求最小值

### Q: TwoSum (给定数组 及sum; 求数组中 和为sum的两个元素)
+ 先排序 后双向扫描法:
    1. 排序 O(n * log2n)
    2. 两个指针分别指向 头尾两端, 向中间扫描 O(n)
+ Hash字典方法:
    1. 构造Hash字典 O(n)
    2. 遍历i, `mHashMap.get(sum - a[i])` 是否为空
    3. 注意 `sum == 2*a[i]` 的重复情况

### Q: K-Sum (给定数组 及sum; 求数组中 和为sum的任意个元素)
+ NP问题:
    1. 标记数组 `bool * bseq = new bool[n]` 每个元素表示 是否使用 对应位置的数组元素
    2. 递归求解 O(2^n)
    3. 实际上是 二叉树
+ 优化1: 假定全为正数
+ 剪枝: (分支限界限)
    + has 表示 0~i-1元素 **由bseq确定的** 的和
    + residue 表示 i+1~n 个元素的和
    + i元素 可以 不使用的 充分条件: `has + residue >= sum `
    + i元素 可以 使用的 充分条件: `has + a[i] <= sum and has + a[i] + residue >= sum`
    + 从而减少 i元素 节点的递归分裂
+ 优化2: 假定存在负数
    1. 对数组 进行 正负分割(partition) 负数在前正数在后
    2. 求正数部分的和 positive_sum
    3. `a[i] < 0` 时 `has + positivie_sum >= sum` i元素可以 使用
    4. `a[i] < 0` 时 不可以使用 的支 不能减掉
    5. `a[i] > 0` 时 按照全是正数处理

### Q: 三元partition
+ 三个指针 `begin, curr, end = 0, 0, size - 1`
+ 判断 `a[curr]`的值 作出对应 操作
+ 遍历条件 `curr <= end`
+ 每次遍历结束后 必然满足:
    1. 区间 [0, begin)    为 `labels[0]`
    2. 区间 (end, size-1] 为 `labels[2]`
    3. 推论 `a[begin] == labels[1]`
    4. 推论 `a[end] != labels[1]`
    5. 存在 `a[end] == labels[0]` 的可能性

### Q: 二元partition
+ 两个指针 `begin, end = 0, size - 1`
+ while begin 不等于 end
    + 如果 `a[begin] == labels[1]`, 交换后 end--
    + 否则 begin++

### Q: 完美洗牌