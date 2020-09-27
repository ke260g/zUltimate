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
    ListNode* removeElementsEx(ListNode* head, int val) {
        if (!head) return head;
        head->next = removeElementsEx(head->next, val);
        if (head->next && head->next->val == val)
            head->next = head->next->next;
        return head;
    }
    // 1. 先删除开头 删除 head == val 的
    // 2. 再递归回溯 删除 next == val 的next节点
    ListNode* removeElements(ListNode* head, int val) {
        if (!head) return head;
        while (head && head->val == val)
            head = head->next;
        return removeElementsEx(head, val);
    }
};

// Remove all elements from a linked list of integers that have value val.