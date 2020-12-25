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
    // 1. 递归回溯; 求得做后到前的索引; 索引从0开始
    // 2. 结果 += val * (2 << (索引-1))
    int getDecimalValueEx(ListNode *head, int *idx) {
        if (!head->next) {
            return head->val;
        }
        int value = getDecimalValueEx(head->next, idx);
        (*idx)++;
        return value + head->val * (2 << (*idx - 1));
    }
    int getDecimalValue(ListNode* head) {
        int idx = 0;
        return getDecimalValueEx(head, &idx);
    }
};

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