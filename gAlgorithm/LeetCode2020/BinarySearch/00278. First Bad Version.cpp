// You are a product manager and currently leading a team to develop a new product.
// Unfortunately, the latest version of your product fails the quality check. Sinc
// e each version is developed based on the previous version, all the versions after a bad version are also bad.

// Suppose you have n versions [1, 2, ..., n] and you want to find out the first bad one, 
// which causes all the following ones to be bad.

// You are given an API bool isBadVersion(version) which returns whether version is bad. 
// Implement a function to find the first bad version. You should minimize the number of calls to the API.

// Input: n = 5, bad = 4
// Output: 4
// Explanation:
// call isBadVersion(3) -> false
// call isBadVersion(5) -> true
// call isBadVersion(4) -> true
// Then 4 is the first bad version.

// Input: n = 1, bad = 1
// Output: 1

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/first-bad-version
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

// 自己写的; 二分查找法
// 形同 00069. Sqrt(x) 题
// 坑1: 刚开始 ans = n; 因为刚开始知道了 最后一个版本肯定是 bad 的
// 坑2: 取的 bad 版本后; 更新一下 ans = min(ans, mid);
class Solution {
public:
    int firstBadVersion(int n) {
        int ans = n;
        int l = 1, r = n;
        while (l <= r) {
            int mid = l + (r - l)/2;
            if (isBadVersion(mid)) {
                ans = min(ans, mid);
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return ans;
    }
};