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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // TODO: 不处理已经成环的 list(可以提前分别做成环判断)
        // 1. 尾节点相等为空且相等; 则相交; 否则不相交
        // 2. 尾节点于a链表头成环
        // 3. 问题变成 链表头b的有环链表的环入口
        if (!headA || !headB) return NULL;
        // 步骤1
        ListNode *a = headA, *b = headB;
        while(a && a->next)
            a = a->next;
        while(b && b->next)
            b = b->next;
        if (b != a) return NULL; 
        
        // 步骤2
        a->next = headA;

        // 步骤3 找相遇点
        ListNode *fast = headB, *slow = headB;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow)
                break;
        }
        // 步骤4 找环入口
        ListNode *head = headB;
        while (head && slow) {
            if (head == slow)
                break;
            slow = slow->next;
            head = head->next;
        }
        // 步骤5: 还原
        a->next = NULL;
        return head;
    }
};