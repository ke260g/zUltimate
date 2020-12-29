// Given a sorted linked list, delete all nodes that have duplicate numbers, 
// leaving only distinct numbers from the original list.

// Return the linked list sorted as well.

// Input: 1->2->3->3->4->4->5
// Output: 1->2->5

// Input: 1->1->1->2->3
// Output: 2->3

// 不用栈回溯
class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return head;
        if (head->next && head->next->val == head->val) {
            int val = head->val;
            head = head->next->next; // 先干掉当前的两个
            while (head && head->val == val) // 再干掉后面的
                head = head->next;
            head = deleteDuplicates(head);
        } else {
            head->next = deleteDuplicates(head->next);
        }
        return head;
    }
};