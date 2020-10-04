/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
// 翻转链表(基本操作; 不多说)
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if (!head) return head;
        ListNode *tail = nullptr;
        ListNode *iter = head;
        while (iter) {
            ListNode *next = iter->next;
            iter->next = tail;
            tail = iter;
            iter = next;
        }
        return tail;
    }
};