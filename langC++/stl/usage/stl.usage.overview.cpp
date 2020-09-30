// vector
vector_usage() {
    vec.clear(); // 删除所有成员; 保留分配的空间 vec.capacity()
    vec.shrink_to_fit(); // 不怎么用; 把capacity缩减到成员个数

    int front = vec.front(); // 首成员
    int back = vec.back();   // 末成员

    vec.push_back(100); // 尾插入
    while (!vec.empty()) { // 尾遍历再销毁
        int elem = vec.back();
        vec.pop_back(); // 尾弹出
    }
    // vec 只有尾遍历 没有头遍历
}

// list
list_usage() {
    list<int> l1;
    l1.clear(); // 删除所有成员; 保留分配的空间 vec.capacity()

    int front = l1.front();
    int end   = l1.end();
    l1.push_back(100);  // 尾插入
    l1.push_front(100); // 头插入

    // unique 已排序list去重
    bool same_func(int l, int r) { return l == r; }
    l1.sort();
    l1.unique(); // 标准变量类型可以不写比较函数
    l1.unique(same_func);

    // merge
    // 把l2 合并到l1 中 LeetCode 00021. Merge Two Sorted Lists
    list<int> l1, l2;
    bool comp_func(int l, int r) { return l < r; }
    l1.merge(l2, comp_func); 

    // splice
    // 把 l2 整个链表所有成员  移动到 l1_iterator+i 前面
    l1.splice(l1_iterator+i, l2); 
    // 把 l2 的l2_iterator+j 这个成员; 移动到 l1_iterator+i 前面
    l1.splice(l1_iterator+i, l2, l2_iterator+j);
    // 把 l2 的l2_iterator+j 到 l2_iterator+k；移动到 l1_iterator+i前面
    l1.splice(l1_iterator+i, l2, l2_iterator+j, l2_iterator+k);

    // remove_if
    bool remove_if_odd(const int & v) { return (v & 0x1) == 1; }
    l1.remove_if(remove_if_odd); // 删除整个链表符合条件的成员

    // LeetCode 00103. Binary Tree Zigzag Level Order Traversal
    while (!l1.empty()) { // 尾遍历再销毁
        int elem = l1.back();
        l1.pop_back();
    }
    while(!l1.empty()) { // 头遍历再销毁
        int elem = l1.front();
        l1.pop_front();
    }
}

// queue
queue_usage() {
    queue<int> q;

    // LeetCode 00102. Binary Tree Level Order Traversal
    q.push(1); // 入列
    while (!q.empty()) { // 出列遍历
        int elem = q.front();
        q.pop();         // 出列
    }
}

// priority_queue
priority_queue_usage() {
    class comp {
    public:
        bool operator() (const int &l, const int &r) const {
            return r < l /* 降序 */; 
            // return l < r /* 升序 */;
        }
    };
    int a[] = {10, 60, 50, 20};
    // 默认是 vector 升序
    priority_queue<int> pq(a, a+4);
    // 升序
    priority_queue<int, vector<int>, less<int>> pq_lt(a, a+4);
    int min_val = pq_lt.top(); // 得到最小值
    pq_lt.pop(); // 取出首元素, 升序即最小值
    // 降序
    priority_queue<int, vector<int>, greater<int>> pq_gt(a, a+4);
    int max_val = pq_gt.top(); // 得到最大值
    pq_gt.pop(); // 取出首元素, 降序即最大值

    // 自实现比较函数; 主要是重定向 () 操作符
    priority_queue<int, vector<int>, comp> pq_lt(a, a+4); // 升序
    priority_queue<int, vector<int>, comp> pq_gt(a, a+4); // 降序
}

// stack
stack_usage() {
    stk<int> stk;

    // LeetCode 00102. Binary Tree Level Order Traversal
    stk.push(1); // 入列
    while (!stk.empty()) { // 出列遍历
        int elem = stk.top();
        q.pop();         // 出列
    }
}


// bitset
bitset_usage() {
    bitset<8> bs;            // 初始化为0
    bitset<8> bs(0xef);      // 用 数值初始化
    bitset<8> bs("0011324"); // 用字符串初始化

    int count_set_bit = bs.count(); // 返回bit为1的个数
    int count_clr_bit = bs.size() - bs.count(); // 返回bit为0的个数
    bool any_bit_set = bs.any();    // 任一bit为1
    bool none_bit_set = bs.any();   // 所有bit为0

     // pos 从左往右 从0开始；
    bool pos_is_set = bs.test(pos); // 是否被设置了
    bs.flip(pos); // 反转一个bit
    bs.flip();    // 反转所有bit
    string bit_string = bs.to_string();
    unsigned long bit_value = bs.to_ulong();

    bs.set(pos);   // 置位
    bs.set();      // 置位全部bit
    bs.reset(pos); // 清零
    bs.reset();    // 清零全部bit
    // 直接设置
    bs[0] = bs[bs.size()-1]; // 最低位 = 最高位
}

// set
set_usage() {
    set<int> set;
    set.insert(1); // 插入集合

    // 查找第一个匹配的元素 并 删除
    set<int>::iterator it = set.find(100);
    do_on(*it);
    set.erase(it);
    // 删除指定一个指定元素
    set.erase(set.find(1000));

    set.count(100); // set 中100的个数；都是1
}

// map
map_usage() {
    map<string, int> m = {{ "Amy", 10 }, { "Sarah", 20 }};
    pair<map<char, int>::iterator, bool> retval;

    m.insert(pair<string, int>("John", 100));
    m.insert(pair<string, int>("Mike", 200));
    
    retval = m.insert(pair<string, int>("Mike", 300)); 
    if (!retval->second) { // 插入失败
        printf("key = %s is already existed, with %d\n",
            retval->first->first, retval->first->second);
    }
    m.erase('Mike'); // 根据 key 删除
    
    map<char, int>::it = m.find("John");
    if (it != m.end()) { // 找不到会返回 m.end()
        printf("find %s with %d\n", it->first, it->second);
    }

    // 访问 Q: [] 和 at 有什么不同?
    m["John"] = 2000;
    m.at("Mike") = 1000;
}