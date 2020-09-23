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
    ListNode* __addTwoNumbers(ListNode* l1, ListNode *l2, int give_next) {
        // 3个变量; 8种场景
        // 1. 两链表为空; 进位为0; 返回空
        // 2. 两链表为空; 进位为1; 返回新节点 val = 1
        // 3. 左链表为空; 右链表不空; 进位为0; 返回右边链表
        // 4. 左链表为空; 右链表不空; 进位为1; 右边链表节点加1; 递归
        // 5. 右链表为空; 左链表不空; 进位为0; 返回左边链表
        // 6. 右链表为空; 左链表不空; 进位为1; 左边边链表节点加1; 递归
        // 7. .8 左右链表不空; 进位为1 / 0, 左右节点相加再加上进位; 递归
        if (!l1 && !l2) // 情况1,2 语法压缩
            return give_next ? new ListNode(1) : NULL;
        if (l1 && l2) { // 情况7,8 语法压缩
            l1->val += l2->val + give_next;
            if (l1->val >= 10) {
                give_next = 1;
                l1->val -= 10;
            } else {
                give_next = 0;
            }
            l1->next = __addTwoNumbers(l1->next, l2->next, give_next);
            return l1;
        }
        ListNode *l = l1 ? l1 : l2;
        if (!give_next) // 情况3, 5
            return l;

        l->val += give_next; // 情况4, 6
        if (l->val >= 10) {
            give_next = 1;
            l->val -= 10;
        } else {
            give_next = 0;
        }
        l->next = __addTwoNumbers(NULL, l2->next, give_next);
        return l;
    }
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        return __addTwoNumbers(l1, l2, 0);
    }
};