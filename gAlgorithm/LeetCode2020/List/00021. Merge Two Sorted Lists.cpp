// Merge two sorted linked lists and return it as a new sorted list. 
// The new list should be made by splicing together the nodes of the first two lists.

// Input: l1 = [1,2,4], l2 = [1,3,4]
// Output: [1,1,2,3,4,4]

// Input: l1 = [], l2 = []
// Output: []

// Input: l1 = [], l2 = [0]
// Output: [0]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/merge-two-sorted-lists
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        // 1. 左链表为空; 返回右链表
        // 2. 右链表为空; 返回左链表
        // 3. 左节点较少; 则左节点的下一节点 归并 右链表; 返回左链表
        // 4. 右节点较少; 则右节点的下一节点 归并 左链表; 返回右链表
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1-> val > l2->val)
            swap(l1, l2);
        l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    }
};