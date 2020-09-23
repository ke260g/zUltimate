/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        // 1. 空节点返回空
        // 2. 链表只有一个节点; 返回当前节点
        // 3. 链表有两个节点:
        //    3.1 递归交换第三个节点后的链表
        //    3.2 当前节点 的next 指向 "第三个节点后的链表"
        //    3.3 下一节点 的next 指向 当前节点
        //    3.4 返回下一节点
        if (!head)
            return NULL;
        if (!head->next)
            return head;

        ListNode *next = head->next;
        head->next = swapPairs(next->next);
        next->next = head;
        return next;
    }
};