Combination Sum

1. 给定一个无重复元素的数组 candidates 和一个目标数 target ，
2. 找出 candidates 中所有可以使数字和为 target 的组合。
3. candidates 中的数字可以无限制重复被选取。

我的思路:
1. 排序; 降序
2. 数值 v[i] 
    1. 如果大于 target；则continue
    2. 如果 ==  target; 则直接为 1 个 v[i]; 
    3. 如果 < target 且 target % v[i] == 0；则结果为 n 个 v[i]
    4. 如果 < target 且 则取模; 把模作为 target 递归
        把递归低到的 vector_remain; 并入到当前的vector中
        递归无法得到 vector_remain；说明数值无法构造 target
    5. 
+ 一定是先有 2. 再有 3.

他的思路:
1. remain 作为树的节点 进行深度搜索(递归)
2. target = 0; 为递归结束
3. 优化: 升序 + 剪枝(提前 break)