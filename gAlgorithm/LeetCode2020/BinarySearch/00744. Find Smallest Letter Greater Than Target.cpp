// Given a list of sorted characters letters containing only lowercase letters, 
// and given a target letter target,
// find the smallest element in the list that is larger than the given target.

// Letters also wrap around. For example, 
// if the target is target = 'z' and letters = ['a', 'b'],
// the answer is 'a'. 

// Input:
// letters = ["c", "f", "j"]
// target = "a"
// Output: "c"

// Input:
// letters = ["c", "f", "j"]
// target = "c"
// Output: "f"

// Input:
// letters = ["c", "f", "j"]
// target = "d"
// Output: "f"

// Input:
// letters = ["c", "f", "j"]
// target = "g"
// Output: "j"

// Input:
// letters = ["c", "f", "j"]
// target = "j"
// Output: "c"

// Input:
// letters = ["c", "f", "j"]
// target = "k"
// Output: "c"

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/find-smallest-letter-greater-than-target
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 二分查找法
// 坑1: 找不到返回首个字母
class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        int l = 0, r = letters.size()-1;
        int ans = r;
        // 找不到; 返回首个字母
        if (letters[r] <= target)
            return letters[0];
        // 二分查找
        while (l <= r) {
            int m = l + (r-l)/2;
            if (letters[m] > target) { // 必须大于而不是大于等于
                ans = min(ans, m);
                r = m-1;
            } else {
                l = m+1;
            }
        }
        return letters[ans];
    }
};