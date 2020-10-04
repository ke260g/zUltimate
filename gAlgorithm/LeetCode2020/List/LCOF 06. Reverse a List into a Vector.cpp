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
    vector<int> *reversePrintCore(ListNode *head) {
        if (!head)
            return new vector<int>();
        vector<int> *result = reversePrintCore(head->next);
        result->push_back(head->val);
        return result;
    }
    vector<int> reversePrint(ListNode* head) {
        return *reversePrintCore(head);
    }
};