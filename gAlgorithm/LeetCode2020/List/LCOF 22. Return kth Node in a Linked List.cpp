// 输入一个链表，输出该链表中倒数第k个节点。
// 为了符合大多数人的习惯，本题从1开始计数，即链表的尾节点是倒数第1个节点。
// 例如，一个链表有6个节点，从头节点开始，它们的值依次是1、2、3、4、5、6。
// 这个链表的倒数第3个节点是值为4的节点。
//
// 给定一个链表: 1->2->3->4->5, 和 k = 2.
// 返回链表 4->5.
// 


class Solution {
public:
    ListNode* getKthFromEndEx(ListNode* head, int k, int *idx) {
        // 1. 递归回溯返回 索引
        // 2. 如果当前节点索引 不大于 k; 则返回自己
        // 3. 否则返回下家
        // 实际上; 从索引k-1的节点开始; 返回索引k的节点; 直到递归栈开始
        if (!head) {
            *idx = 0;
            return head;
        }
        head->next = getKthFromEndEx(head->next, k, idx);
        (*idx)++;
        if (*idx <= k)
            return head;
        return head->next;
    }
    ListNode* getKthFromEnd(ListNode* head, int k) {
        int idx = -1;
        return getKthFromEndEx(head, k, &idx);
    }
};