/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
// equals to LeetCode 00142 Linked List Cycle II
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *a = headA, *b = headB;
        if (!headA || !headB) return NULL;
        while (a->next)
            a = a->next;
        while (b->next)
            b = b->next;
        if (a != b)
            return NULL; // 没有交点
        a->next = headA; // 成环

        ListNode *fast = headB, *slow = headB;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
            if (fast == slow)
                break;
        }
        ListNode *curr = headB;
        while (curr && slow) {
            if (curr == slow)
                break;
            curr = curr->next;
            slow = slow->next;
        }
        a->next = NULL;  // 破环; 恢复
        return curr;
    }
};