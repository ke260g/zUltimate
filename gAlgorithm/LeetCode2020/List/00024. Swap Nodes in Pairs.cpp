// Given a linked list, swap every two adjacent nodes and return its head.

// You may not modify the values in the list's nodes. Only nodes itself may be changed.

来源：力扣（LeetCode）
链接：https://leetcode-cn.com/problems/swap-nodes-in-pairs
著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

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
    ListNode* swapPairs(ListNode* head) {
        // 1. 空节点返回空
        // 2. 链表只有一个节点; 返回当前节点
        // 3. 链表有两个节点:
        //    3.1 递归交换第三个节点后的链表
        //    3.2 当前节点 的next 指向 "第三个节点后的链表"
        //    3.3 下一节点 的next 指向 当前节点
        //    3.4 返回下一节点
        if (!head)
            return NULL;
        if (!head->next)
            return head;

        ListNode *next = head->next;
        head->next = swapPairs(next->next);
        next->next = head;
        return next;
    }
};

// 不用递归
class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        if (!head)
            return NULL;
        if (!head->next)
            return head;
        ListNode *tail = nullptr;
        ListNode *ans = head->next;
        ListNode *iter = head;
        while (iter && iter->next) {
            ListNode *next = iter->next;
            iter->next = next->next;
            next->next = iter;
            
            if (tail) tail->next = next;
            tail = iter;
            iter = iter->next;
        }
        return ans;
    }
};