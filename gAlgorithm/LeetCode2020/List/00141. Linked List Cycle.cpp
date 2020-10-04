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
    // 4. 最终; fast不能走两步; 无环
    bool hasCycle(ListNode *head) {
        ListNode *fast = head, *slow = head;
        while(fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
            if (fast == slow)
                return true;
        }
        return false;
    }
};