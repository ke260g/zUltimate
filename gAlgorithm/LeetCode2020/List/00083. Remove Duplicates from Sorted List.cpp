// Given a sorted linked list, delete all duplicates such that each element appear only once.

// Input: 1->1->2
// Output: 1->2

// Input: 1->1->2->3->3
// Output: 1->2->3

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/remove-duplicates-from-sorted-list
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
    // 递归
    // 如果后继结点有值且等于自己; 干掉自己
    // 否则返回自己
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) return head;
        head->next = deleteDuplicates(head->next);
        if (head->next && head->next->val == head->val)
            return head->next;
        return head;
    }
};