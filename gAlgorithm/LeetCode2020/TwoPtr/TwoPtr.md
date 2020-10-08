[TOC]
双指针问题
# 1. 滑动窗口型
00026. Remove Duplicates from Sorted Array
    滑动窗口是 重复元素; 窗口左边是没有重复的序列
00027. Remove Element
    滑动窗口是 待删除元素; 窗口左边是不删除的序列
00283. Move Zeroes
    00027 的特例; 待删除元素指定是zero
00075. Sort Colors
    这是荷兰国旗问题; 虽然这个问题本质形同 快排;
    但是快排经典写法是 前后逼近; 不是滑动窗口
    这个问题的滑动窗口是 中间的颜色
    后边的颜色属于特殊处理
    技术上 快排也可以写成滑动窗口; 窗口内是 大于等于pivot的元素
## 算法分析
1. 一般用于一个数组
2. 定义一个条件; 满足条件的属于窗口中; 不属于条件的在窗口前面
    + 荷兰国旗问题还会把不符合条件的放到后面
3. 滑动窗口时
    1. `[0, 条件边界)` 表示不在窗口中的元素
    2. `[条件边界, i)` 表示滑动的窗口
4. 元素满足条件 `i++` 扩大窗口
5. 元素不满足条件 `swap(a[i++], a[条件边界++])` 先交换后移动
## 算法模板
```c++
int cond_edge = 0, i = 0;
while (i < a.size()) {(
    if (isMatch(a[i]))
        i++;
    else
        swap(a[i++], a[cond_edge++]);
}
```

# 2. 前后逼近型
快序排序是经典的前后逼近型; 逼近条件是与 pivot 的大小比较
00125. Valid Palindrome;    逼近条件是 `isalnum()`
00167. Two Sum II - Input array is sorted
00344. Reverse String
    虽然我们用了无脑的 下标偏移求要交换的位置
    但是这个问题本质上还是前后逼近型
00345. Reverse Vowels of a String; 逼近条件是isVowels()
LCOF 04. Find in 2D Matrix  逼近条件是target在单个vector中

## 算法分析
1. 一般用于一个数组
2. 单次循环中; 前后指针根据"逼近条件"找到分别复合条件的元素; 然后进行操作
    + 模板中 逼近条件 用 isNotMatch表示
3. 注意; 单次循环中; 前后指针逼近时也要进行同样的循环条件判断
## 算法模板
```c++
int head = 0, tail = a.size()-1;
while (head < tail) {
    while(isNotMatch(a[head]) && head < tail)
        head++;
    while(isNotMatch(a[tail]) && head < tail)
        tail--;
    doOn(a[head], a[tail]);
    head++;
    tail--;
}
```

# 3. 归并型
00088. Merge Sorted Array
00349. Intersection of Two Arrays    (交集要单值化)
00350. Intersection of Two Arrays II (交集不单值化)
00925. Long Pressed Name
00977. Squares of a Sorted Array
LCCI 10.01. Sorted Merge Array
## 算法分析
1. 用于两个数组 可能是按某条件合并为一个
2. 没看出共性

# 4. 单向链表找节点
1. 找中间节点
2. 找倒数第n个节点
3. 找倒数第n+1个节点
    1. n在长度范围中;
    2. 当n等于长度; 即可目标节点就是head; n+1超出范围后特殊处理
4. 链表成环找交点
5. 翻转 (翻转在形式上是双指针)