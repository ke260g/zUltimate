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