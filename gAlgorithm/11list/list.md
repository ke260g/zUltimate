## Q: 单向链表的 部分翻转 m ~ n 部分
+ 头插法
1. 找点第m个节点 几位head
2. 以head为起点 遍历n-m次
    + 将找到的节点 插入到 head的next

## Q: 单向链表的 二元分割
+ T(n) = O(n)
+ S(n) = O(1)
+ 即快速排序可以应用于 单向链表
+ 方法:
    1. 两个单向链表 头指针 pl pr
    2. 小于 pivot的添加到 pl链表后面
    3. 大于 pivot的添加到 pr链表后面
    4. 注意pl的尾要接上pr的头

## Q: 删除已排序 单向链表 中的重复元素
+ 保留重复元素的一个
+ 不保留重复元素

# Q set 一个单链 成环问题
```c++
fast = slow = head;
loop_node_cnt = 0;
while (fast->next != NULL) {
    fast = fast->next;
    if (!fast) break;
    slow = slow->next;
    fast = fast->next;
    if (slow == fast) break;
}
if (slow == NULL || fast -> next == NULL) return NULL ; //没有环
has_loop = true;

Node * ptr1 = head ; //链表开始点
Node * ptr2 = slow ; //相遇点
while (ptr1 != ptr2) 
{
    ptr1 = ptr1 -> next ;
    ptr2 = ptr2 -> next ;
}
in_point = ptr1 ; //找到入口点

fast = fast->next;
while (fast != slow) {
    loop_node_cnt++;
    fast = fast->next;
}
```
## Q: 一个单链表，判断其中是否有环存在
两个指针 fast slow; 从头开始; 一个一次走两步; 一个一次走一步; 
如果成环：则两个指针最终会相遇; 否则指向NULL；结束;
## Q: 如果存在环，找出环的入口点
两个指针 ptr1 ptr2; 一个从头开始一次走一步; 一个从相遇点开始一次走一步;
重新相遇; 就是环入口;
```log
设 链表总节点个数 s = x + y; w 指示 环入口到首次相遇点的节点个数
x 指示 成环前的节点数; y 指示 环节点个数
1. 因为 fast 指针比 slow 指针先进入环; 相遇时 slow 一定没有走完一圈
   且 fast走的节点数 = 2 * slow走的节点数
   而 fast走到节点数 = x + w + 多个环; slow走的节点数 = x+w
   x + n*y + w = 2*(x + w)
2. n*y = x + w 化解
3. (n-1)*y + (y - w) = x 继续化解
因此: 环节点数 - 环入口到相遇点的节点数 = 成环前节点数
所以: 从相遇点开始往前走 同时 从起点开始走 => 再次相遇即环入口
```
## Q: 如果存在环，求出环上节点的个数
前两个步骤中; 记录两个相遇点;
起点~fast和slow相遇点的节点数 + 
起点~环入口节点数 = 环节点数
## Q: 如果存在环，求出链表的长度
环节点数 + 起点~环入口节点数 = 链表长度
## Q: 如果存在环，求出环上距离任意一个节点最远的点
走环节点个数的 一半的个数

# Q set 两个单链 成环问题
## Q: 两单向链表各自无环 如何判断是否相交 以及 找到首个交点
1. 末尾节点相等; 即相交;
2. 把一条单链构造成环; 问题退还为 单链成环 求交点问题

## Q: 两单向链表各自有环 如何判断是否相交 以及 找到首个交点
??