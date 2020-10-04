## 链表快慢指针应用
1. 链表成环问题
2. 链表中点问题( 00234 / 00876 )
    1. 00234. Palindrome Linked List
    2. 00876. Middle of the Linked List

## 链表成环
00141. Linked List Cycle                   判断是否有环
00142. Linked List Cycle II                求出环的入口
00160. Intersection of Two Linked Lists    判断是否相交
LCOF 52. Cross Node of Two Linked List     求出相交的点

## 链表翻转 (缓存next > iter后接tail > ter作为tail > iter步进)
00234. Palindrome Linked List 判断对称链表; 快慢指针找中点; 把后半链表翻转; 然后正向遍历
LCOF 25. Reverse Linked List

## 先操作当前节点; 再递归操作next 子链表
00002. Add Two Numbers         双链表形态
00021. Merge Two Sorted Lists  双链表形态
00024. Swap Nodes in Pairs     单次操作两个节点 (本质上可以操作N个; N < 长度)

## 先递归操作next 子链表; 再操作当前节点 (正向求长度 逆向递归回溯求索引)
00019. Remove Nth Node From End of List
LCOF 22. Return kth Node in a Linked List
01290. Convert Binary Number in a Linked List to Integer
00061. Rotate List  旋转本质: 链表成环 > 根据索引找到新的头 > 新的头的前驱破环