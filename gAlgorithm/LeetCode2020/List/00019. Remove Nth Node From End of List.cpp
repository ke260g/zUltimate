// 删除数组倒数第k个元素

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
        while (n >= 0 /*正常找倒数第n个 不需要等于0*/ && fast) {
            fast = fast->next;
            n--;
        }
        // n == 0 表示倒数第n个节点就是头节点; n+1个数还没找到fast就为nullptr
        if (n == 0) return head->next;
        while (fast) {
            fast = fast->next;
            slow = slow->next;
        }
        slow->next = slow->next->next;
        return head;
    }
};