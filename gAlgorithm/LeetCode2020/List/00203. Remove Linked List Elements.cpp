// Remove all elements from a linked list of integers that have value val.

// Input:  1->2->6->3->4->5->6, val = 6
// Output: 1->2->3->4->5

class Solution {
public:
    ListNode* removeElements(ListNode* head, int val) {
        if (!head) return head;
        head->next = removeElements(head->next, val);
        return head->val == val ? head->next : head;
    }
};