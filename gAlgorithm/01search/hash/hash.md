[TOC]
hash 扩容的过程
hash 用链表解决冲突时; 还是冲突怎么办: 扩容
hash 和 array 的优缺点比较
为什么 bucket个数是2的次方大小时，hash的效率最高

java HashMap  https://zhuanlan.zhihu.com/p/151027796
hash 入门讲解 https://zhuanlan.zhihu.com/p/144296454

# 1. hash 函数
## 1.1 直接定址法; (线性函数)
`H(key) = a * key + b (其中a, b可以为 0)`

## 1.2 除留余数法; (质数取模)
`H(key) = key % p (其中 p ≤ m)`

## 1.3 平方取中法;
关键字值的平方，然后取平方值中间几位作为散列地址
取的位数由表长决定

### 折叠法
将关键字分为位数相同的几部分，然后取这几部分的叠加和（舍去进位）作为散列地址。

# 2. hash 冲突解决
## 2.1 拉链法
1. 优点
    1. 插入快速; 无堆积(非key不会发生冲突) 因此平均查找长度较短
    2. 各链表上的结点空间是动态申请的，故它更适合于造表前无法确定表长的情况
    3. 在用拉链法构造的散列表中，删除结点的操作易于实现
2. 缺点
    1. 
## 2.2 开放地址法
1. 位桶不需要链表来实现 装载因子不会超过1
2. 它的实现是在插入一个元素，先通过哈希函数进行判断，
3. 若是发生哈希冲突，就以当前地址为基准，根据再寻址的方法（探查序列），去寻找下一个地址
4. 若发生冲突再去寻找，直至找到一个为空的地址为止

### 2.2.1 线性探测LP
1. ` hi = ( h(key) + i ) ％ m    其中 (0 ≤ i ≤ m-1)`
2. 基本思想:
    1. 探查时从地址 d `(h(key))` 开始
    2. 首先探查 T[d]，然后依次探查 T[d+1]，…，直到 T[m-1]
    3. 此后又循环到 T[0] T[1] ...
    4. 直到探查到 有空余地址 或者到 T[d-1]为止

### 2.2.2 二次探测QP
1. ` hi = ( h(key) + i*i ) ％ m  其中 (0 ≤ i ≤ m-1)`
2. 基本思想： 
    1. 探查时从地址 d `(h(key))` 开始
    2. 首先探查 T[d]，然后依次探查 T[d+1^2] T[d+2^2] T[d+3^2] ...
    3. 直到探查到 有空余地址 或者到 T[d-1]为止
3. 缺点是无法探查到整个散列空间

### 2.2.3 双倍散列DP
1. `hi=( h(key) + i*h1(key) ) ％ m，其中 (0 ≤ i ≤ m-1)` 
2. 基本思想 
    1. 探查时从地址 d ( h(key)) 开始
    2. 首先探查 T[d]，然后依次探查 `T[d+h1(d)]` `T[d + 2*h1(d)]` ...
3. 该方法使用了两个散列函数 h(key) 和 h1(key)，故也称为双散列函数探查法。
4. Note: 必须使 h1(key) 的值和 m 互素，才能使发生冲突的同义词地址均匀地分布在整个表中，否则可能造成同义词地址的循环计算。
5. Note: 该方法是开放定址法中最好的方法之一

## 2.3 再散列法

## 2.4 公共溢出区

# 3. 字符串 hash
https://oi-wiki.org/string/hash/
https://cp-algorithms.com/string/string-hashing.html
https://byvoid.com/zht/blog/string-hash-compare/

1. 把字符串映射到整数
2. 映射后方便判断两字符串是否相等
    1. 即 hash值 不一样时 字符串一定不相同
    2. 但字符串不相同时, hash 值可以一样 (冲突)
## 3.1 字符串 hash 例子1
```c++
const int M = 1e9 + 7;
const int B = 233;
int get_hash(const string& s) {
    int res = 0;
    for (int i = 0; i < s.size(); ++i) {
        res = (long long)(res * B + s[i]) % M;
    }
    return res;
}
```
1. (每一个字符 + base * 乘上个字符的摸) 然后取模
    + 目的: hash值与字符所在的位置有关
2. Base 与 Module 必须互质; 否则槽位 Module 上分布概率不均匀
    + 数学原理不是很理解
3. 一般地, Module 会选择较大的质数

### 版本2:


# 4. hash 扩容
## 4.1 redis 的 dict 实现
https://www.cnblogs.com/mfrbuaa/p/5245064.html
注意看后半部分
1. 关键变量
    1. `struct dictht` 里面包含了mask 用来取模; 代表hash函数
    2. `dictht ht[2];` ht[0] 旧桶; ht[1] 正在扩展的新桶
    3. `d->rehashidx` 迁移游标
2. 负载因子超过阈值;
    1. 扩容: ht[1] = new dictht(ht[0].size)
    2. 迁移因子 = 0
3. get:
    1. 如果完成迁移: 在 `ht[0]` 中查找
    2. 如果正在迁移: 在 `ht[1]` 中查找
                     找不到 且 hash值 小于 d->rehashindex; (说明可能还在 `ht[0]`中)
                     在 `ht[0]` 中查找
4. put:
    1. 如果完成迁移: 用旧hash函数插入 `ht[0]` 
    2. 如果正在迁移: 用新hash函数插入 `ht[1]`
                 迁移`ht[0].table[d->rehashindx++]` 到 `ht[1]` 中
5. 完成迁移: 
    1. `free(ht[0]); ht[0] = ht[1]; ht[1] = NULL;`
    2. `d->rehashindex = 0;`

## 4.2 自己实现的线性hash
1. 负载因子 `load_factor = size / bucket.size();` 超出阈值; 扩容
    1. `cursor = 0`
2. 直接 `bucket.resize(bucket.size()*2, 0)`
    1. 因为我们是线性hash
    2. 如果用新hash函数求得h 在`新桶[h]`中 且h小于 旧桶大小
    3. 那么一定是在 `旧桶[h]` 中
    4. 即可我们新桶的前半部分等同旧桶; 直接 resize 复用即可; 不需要双`bucket[]`
3. 平滑插入
    1. put: 插入时如果正在扩容; 则用旧的hash函数; 先放到旧桶中
            旧桶`buckets[cursor++]`中的每个元素进行迁移
    2. get: 没有完成迁移时; 用新hash函数求hash值; 如果hash值大于 corsor; 再用旧hash函数求hash值在旧桶中找
            已经完成迁移后; 用旧hash函数求hash值; 在旧桶中找
    3. 完成迁移时; `旧hash函数 = 新hash函数`

## 4.3 c++ stl::map