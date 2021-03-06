// 给定一个带有头结点 head 的非空单链表，返回链表的中间结点。
// 如果有两个中间结点，则返回第二个中间结点。

// 快慢指针;
// 如果是 A B;       使得慢指针指向B 为中间节点
// 如果是 A B C;     使得慢指针指向B 为中间节点
// 如果是 A B C D；  使得慢指针指向C
//   A->B->C->nullptr
//   D->C---->nullptr
// 如果是 A B C D E; 使得慢指针指向C
//   A->B->C->nullptr
//   E->D->C->nullptr
class Solution {
public:
    ListNode* middleNode(ListNode* head) {
        // 快慢指针找中间节点
        // 节点个数是偶数时; slow 节点最后指向中间靠右
        // 节点个数是奇数时; slow 节点最后指向中间节点
        if (!head) return nullptr;
        ListNode *slow = head, *fast = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;

        
    }
};