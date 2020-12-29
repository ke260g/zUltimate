// 删除数组倒数第k个元素
// Given the head of a linked list, remove the nth node from the end of the list and return its head.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/remove-nth-node-from-end-of-list
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 栈回溯求索引
class Solution {
public:
    ListNode* removeNthFromEndEx(ListNode* head, int target, int *idx) {
        if (!head) {
            *idx = 0;
            return head;
        }
        head->next = removeNthFromEndEx(head->next, target, idx);
        *idx += 1; // 当前节点的索引值
        if (target == *idx)
            return head->next;
        return head;
    }
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 单链; 栈回溯
        // 递归返回索引; 当索引等同目标 N; 即删除
        int idx = -1;
        return removeNthFromEndEx(head, n, &idx);
    }
};

// 快慢指针法
class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 先找到前驱; 前驱是倒数n+1个数
        // 快慢指针法找到倒数n+1个节点
        ListNode *fast = head, *slow = head;

        n++; // 这里要倒数第n+1个; 所以先n++
        while (n > 0 && fast) {
            fast = fast->next;
            n--;
        }

        // 说明倒数第n个就是头结点; 使得n无法减为0
        if (n > 0) return head->next;
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }

        slow->next = slow->next->next;
        return head;
    }
};