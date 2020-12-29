// We have a collection of stones, each stone has a positive integer weight.

// Each turn, we choose the two heaviest stones and smash them together.
// Suppose the stones have weights x and y with x <= y.  The result of this smash is:

//     If x == y, both stones are totally destroyed;
//     If x != y, the stone of weight x is totally destroyed, and the stone of weight y has new weight y-x.

// At the end, there is at most 1 stone left.  Return the weight of this stone (or 0 if there are no stones left.)

// Input: [2,7,4,1,8,1]
// Output: 1
// Explanation: 
// We combine 7 and 8 to get 1 so the array converts to [2,4,1,1,1] then,
// we combine 2 and 4 to get 2 so the array converts to [2,1,1,1] then,
// we combine 2 and 1 to get 1 so the array converts to [1,1,1] then,
// we combine 1 and 1 to get 0 so the array converts to [1] then that's the value of last stone.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/last-stone-weight
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 1. 用大顶堆放石头
// 2. 当石头个数 大于 1时 
// 3. 取最重的两块石头   pop_back pop_back
// 4. 把剩下的重量塞回去 push_back
// 5. 重复 1~4
// 6. 如果没有石头了; 那就返回0; 
//    否则返回剩下的那块石头
class Solution {
public:
    int lastStoneWeight(vector<int>& stones) {
        priority_queue<int, vector<int>, greater<int>> maxheap(stones.begin(), stones.end());
        while (maxheap.size() > 1) {
            int x = maxheap.top(); maxheap.pop();
            int y = maxheap.top(); maxheap.pop();
            if (y != x)
                maxheap.push(abs(y - x));
        }
        return maxheap.empty() ? 0 : maxheap.top();    
    }
};