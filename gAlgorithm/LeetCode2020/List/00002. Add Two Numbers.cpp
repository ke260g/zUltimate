// You are given two non-empty linked lists representing two non-negative integers.
// The digits are stored in reverse order, and each of their nodes contains a single digit.
// Add the two numbers and return the sum as a linked list.

// You may assume the two numbers do not contain any leading zero, except the number 0 itself.


// Input: l1 = [2,4,3], l2 = [5,6,4]
// Output: [7,0,8]
// Explanation: 342 + 465 = 807.

// Input: l1 = [0], l2 = [0]
// Output: [0]

// Input: l1 = [9,9,9,9,9,9,9], l2 = [9,9,9,9]
// Output: [8,9,9,9,0,0,0,1]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/add-two-numbers
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

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
        l->next = __addTwoNumbers(NULL, l->next, give_next);
        return l;
    }
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        return __addTwoNumbers(l1, l2, 0);
    }
};

// 简化递归逻辑; 速度会慢一点
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        if (!l1 && !l2) return nullptr;
        if (!l1) swap(l1, l2);
        if (!l2 && l1->val < 10) return l1;

        l1->val += (l2 ? l2->val : 0);
        int plus = l1->val / 10;
        l1->val %= 10;
        if (plus) {
            if (l1->next)
                l1->next->val += plus;
            else
                l1->next = new ListNode(plus);
        }
        l1->next = addTwoNumbers(l1->next, l2 ? l2->next : nullptr);
        return l1;
    }
};