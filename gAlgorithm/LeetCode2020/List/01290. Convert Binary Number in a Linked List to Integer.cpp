/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
// 非递归法
class Solution {
public:
    int getDecimalValue(ListNode* head) {
        // 根据题意, 链表头是最高位, 链表尾是最低位
        int ans = 0;
        for (; head; head = head->next)
            ans = (ans << 1) | head->val;
        return ans;

        // 另外; 如果 链表头是最低位, 链表尾是为高位
        return head ? ((getDecimalValue(head->next) << 1) | head->val) : 0;
    }
};