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
    ListNode* middleNodeEx(ListNode *head, int idx, int *len) {
        // 1. 正向遍历 计算索引, 从0开始
        // 2. 递归回溯 求得长度
        // 3. 当前索引 < 长度/2(向下取整)
        //    即属于中间节点的前半部分节点; 那就继承next节点
        // 4. 否则; 说明属于中间节点的后半部分; 那就返回自己
        idx++;
        if (!head) {
            *len = idx;
            return head;
        }
        ListNode *next = middleNodeEx(head->next, idx, len);
        if (idx < (*len/2))
            return next;
        return head;
    }
    ListNode* middleNode(ListNode* head) {
        int len = 0;
        return middleNodeEx(head, -1, &len);
    }
};