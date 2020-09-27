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
        // 1. 递归栈回溯; value表示"原始链表"的下一个节点的值
        // 2. 当前节点值 == val
        // 2.1 当前节点的下一个节点 ！= val
        //     说明原始链表的下一个节点已被删除; 删除自己即可
        // 2.2 当前节点的下一个节点 == val
        //     删除自己和下一个节点
        // 3. 更新value; 往上传递
        if (!head->next) { // 递归终止条件
            *val = head->val;
            return head;  
        }

        head->next = deleteDuplicatesEx(head->next, val); // 递归开始

        if (*val == head->val) {
            /* 原始链表的下一个节点已被删除; 删除自己 */
            if (!head->next || head->next->val != *val) {
                return head->next;
            } else { /* 当前链表的下一个节点和当前节点相同; 删除两个 */
                return head->next->next;
            }
        } else {
            *val = head->val;
            return head;
        }
    }
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return head;
        int val = 0;
        return deleteDuplicatesEx(head, &val);
    }
};