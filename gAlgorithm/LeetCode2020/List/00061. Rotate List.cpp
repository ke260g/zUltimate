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
    /**
     * @return NULL 表示curr节点不是新的头
     */
    ListNode* rotateRight(ListNode* head, int k) {
        // 1. k = 0 不翻转
        // 2. 空链表 不翻转
        // 3. 单节点链表不翻转
        //
        // 1. 正向计算长度;
        // 2. 新的链表尾巴 位于 head + (len - k % len) 
        // 3. 新的链表尾巴的next 为新的链表头;
        // 4. 链表尾巴破环
        if (k == 0 || !head || !head->next)
            return head;
    
        // 计算长度
        int len = 1;
        ListNode *iter = head;
        for (; iter->next; len++)
            iter = iter->next;
        
        // 旋转系数规整; 避免重复的旋转
        k %= len;
        if (k == 0) return head;

        iter->next = head; // 成环
        iter = iter->next; // 从头开始
        for (int i = 1; i < len-k; ++i)
            iter = iter->next;

        head = iter->next;  // 新链表
        iter->next = nullptr; // 破环

        return head;
    }
};