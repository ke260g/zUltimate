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
    // 1. fast 一次走两步
    // 2. slow 一次走一步
    // 3.1 先走步; 再判断(避免初始值相等)
    // 3.2.当两者相遇; 则有环
    // 3.3 fast 只能走一步; 那就是无环
    // 4. 最终; fast不能走; 无环
    // 5. slow 和 head 指针同时往前走一步 
    // 6. 直到相遇; 相遇点就是环入口 (数学推倒)
    // 7. 无法相遇则无环
    ListNode *detectCycle(ListNode *head) {
        ListNode *fast = head, *slow = head;
        while(fast) {
            slow = slow->next;
            fast = fast->next;
            if (!fast)
                return NULL; // 无环
            fast = fast->next;
            if (fast == slow)
                break;       // 首次相遇点
        }
        while(head && slow) {
            if (head == slow)
                return head;
            head = head->next;
            slow = slow->next;
        }
        return NULL;
    }
};