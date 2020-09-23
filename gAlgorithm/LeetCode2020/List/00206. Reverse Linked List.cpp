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
    ListNode* reverseList(ListNode* head) {
        // 1. 空链表返回空
        // 2. 建立尾巴
        // 3. 缓存链表头的下一个节点
        // 4. 链表头的next指向尾巴
        // 5. 更新尾巴   (尾部指向链表头)
        // 6. 更新链表头 (之前缓存的下一个节点)
        if (!head) return head;
        ListNode *tail = NULL;
        while (head) {
            ListNode *next = head->next;

            head->next = tail;
            tail = head; 

            head = next;
        }
        return tail;
    }
};