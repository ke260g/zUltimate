// 判定单链表是否对称
// 要求 时间复杂度为 O(n) 和 O(1)
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */

// 快慢指针;
// 如果是 A B;       使得慢指针指向A 为中间节点
// 如果是 A B C;     使得慢指针指向B 为中间节点
// 如果是 A B C D；  使得慢指针指向B
// 如果是 A B C D E; 使得慢指针指向C
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        // 1. 通过快慢指针找到中间节点
        // 2. 链表从中间节点断开
        // 3. 反转后半链表
        // 4. 遍历已翻转的后半链表 和 从中间断开的前半链表 判断是否对称

        // 1.
        if (!head) return true;
        if (!head->next) return true;
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        
        // slow 后方就是另一半链表; 把他们反转
        ListNode *prev = head;
        ListNode *post = nullptr;
        ListNode *iter = slow->next; // 遍历后半节点

        // 2.
        slow->next = nullptr; // 断开
        // 3
        while (iter) { // 反转链表
            ListNode *next = iter->next;
            iter->next = post;
            post = iter;
            iter = next;
        }

        // 4.
        while(prev && post) {
            if (prev->val != post->val)
                return false;
            prev = prev->next;
            post = post->next;
        }
        return true;
    }
};

// 递归法传递
// 自己想的正确方案; 递归法 
class Solution {
public:
    bool isPalindromeEx(ListNode *head, ListNode **iter, bool *pass_middle) {
        // 1. 传递链表头部指针递归回溯
        // 2. 递归终止条件, 当且节点为空，
        // 3. 如果后方节点检测到不对称项; 则返回不对称
        // 4. 已经过了中间节点; 则返回后方节点的结果
        // 5. 首次达到后方节点; 则返回后方节点的结果
        // 6. 如果当前节点不对称; 提前返回结果
        // 7. 中间两个是相邻节点; 则标记中间节点
        // 8. 中间两个不时相邻节点; 肯定也不是中间节点(不然提前返回了); iter前移
        if (!head) return true; // 2.
        bool post_res = isPalindromeEx(head->next, iter, pass_middle); // 1.
        if (!post_res) return post_res; // 3.
        // 跑到这里说明后方节点是对称的
        if (*pass_middle || *iter == head) { // 4. 5.
            *pass_middle = true;
            return post_res;
        }
        if (head->val != (*iter)->val) // 6.
            return false;
        // 跑到这里说明当前节点和后方节点都是对称的
        // 遍历到中间节点; 证明是对称的; 提前retun
        if (head == (*iter)->next)
            *pass_middle = true;   // 7.
        else
            *iter = (*iter)->next; // 8.
        return true;
    }
    bool isPalindrome(ListNode* head) {
        if (!head) return true;
        ListNode *iter = head;
        bool pass_middle = false;
        return isPalindromeEx(head, &iter, &pass_middle);
    }
};