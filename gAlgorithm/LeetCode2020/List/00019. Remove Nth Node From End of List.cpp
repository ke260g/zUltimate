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
    ListNode* removeNthFromEndEx(ListNode* head, int target, int *idx) {
        if (!head) {
            *idx = 0;
            return head;
        }
        head->next = removeNthFromEndEx(head->next, target, idx);
        *idx += 1; // 当前节点的索引值
        if (target == *idx)
            return head->next;
        return head;
    }
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 单链; 栈回溯
        // 递归返回索引; 当索引等同目标 N; 即删除
        int idx = -1;
        return removeNthFromEndEx(head, n, &idx);
    }
};