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
    void rotateRightEx(ListNode *head, ListNode *curr, ListNode **result, int k, int *idx, int *len) {
        *len += 1; // 长度
        if (!curr->next) {
            *idx = 1;           // 从尾到头的索引
            if (k % *len == 0) {// 链表不发生旋转
                *result = head;
                return;
            }
            curr->next = head;  // 成环
            *result = ((k % *len) == *idx) ? curr : *result;
            return;
        }
        
        rotateRightEx(head, curr->next, result, k, idx, len);
        *idx += 1; // 当前节点的索引
        if (result && *result == curr->next) { // 破环
            curr->next = NULL;
            return;
        }
        *result = ((k % *len) == *idx) ? curr : *result;
    }
    ListNode* rotateRight(ListNode* head, int k) {
        // 1. k = 0 不翻转
        // 2. 空链表 不翻转
        // 3. 单节点链表不翻转
        //
        // 递归
        // 1. 正向计算长度;
        // 2. 回溯计算索引; (从末尾到开头的索引)
        // 3. k % 长度 == 节点索引; 新链表为当前节点
        // 4. 当前节点为 新的链表的前驱; 破环
        // 5. k % 长度 == 0; 不翻转; 新链表为就链表
        if (k == 0 || !head || !head->next)
            return head;

        int idx = -1, len = 0;
        ListNode *result = NULL;
        rotateRightEx(head, head, &result, k, &idx, &len);
        return result;
    }
};