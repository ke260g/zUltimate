// 给定链表 和 val; 删除等同val的节点
// 别用递归; 太慢了
// 1. 从头开始遍历; 先删除自己后面的
// 2. 最后 如果头节点也需要删除; 那么就删除头节点 
class Solution {
public:
    ListNode* deleteNode(ListNode* head, int val) {
        if (!head) return NULL;
        ListNode *iter = head;
        while (iter->next) {
            if (iter->next->val == val) {
                iter->next = iter->next->next;
                continue;
            }
            iter = iter->next;
        }
        if (head->val == val)
            head = head->next;
        return head;
    }
};