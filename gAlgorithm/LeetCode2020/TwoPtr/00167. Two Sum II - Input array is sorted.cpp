// Given an array of integers that is already sorted in ascending order,
// find two numbers such that they add up to a specific target number.

// The function twoSum should return indices of the two numbers such that they add up to the target, 
// where index1 must be less than index2.

// Note:
//     Your returned answers (both index1 and index2) are not zero-based.
//     You may assume that each input would have exactly one solution and you may not use the same element twice.


// Input: numbers = [2,7,11,15], target = 9
// Output: [1,2]
// Explanation: The sum of 2 and 7 is 9. Therefore index1 = 1, index2 = 2.

// Input: numbers = [2,3,4], target = 6
// Output: [1,3]

// Input: numbers = [-1,0], target = -1
// Output: [1,2]

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/two-sum-ii-input-array-is-sorted
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 已排序数组的 TwoSum 问题
// 经典前后指针法
// 这里有个坑就是题目要求的索引要+1
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        vector<int> match;
        int head = 0,
            tail = numbers.size()-1;
        while (head < tail) {
            if (numbers[head] + numbers[tail] == target) {
                match.push_back(head+1); // 题目要求 索引+1
                match.push_back(tail+1); // 题目要求 索引+1
                return match;
            } else if (numbers[head] + numbers[tail] > target) {
                tail--;
            } else if (numbers[head] + numbers[tail] < target) {
                head++;
            }
         }
        return match;
    }
};