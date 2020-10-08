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
    ListNode* deleteDuplicatesEx(ListNode* head, int *val) {
        // 1. 递归栈回溯; 求的下一个节点的值
        // 2. 如果当前节点 == 下一个节点的值; 则删除自己
        if (!head->next) {
            *val = head->val;
            return head;
        }
        head->next = deleteDuplicatesEx(head->next, val);
        if (*val == head->val) {
            return head->next;
        } else {
            *val = head->val;
            return head;
        }
    }
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head)
            return head;
        int val = 0;
        return deleteDuplicatesEx(head, &val);
    }
};