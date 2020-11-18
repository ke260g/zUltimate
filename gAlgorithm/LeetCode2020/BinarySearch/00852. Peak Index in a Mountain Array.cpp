// Let's call an array arr a mountain if the following properties hold:

//     arr.length >= 3
//     There exists some i with 0 < i < arr.length - 1 such that:
//         arr[0] < arr[1] < ... arr[i-1] < arr[i]
//         arr[i] > arr[i+1] > ... > arr[arr.length - 1]

// Given an integer array arr that is guaranteed to be a mountain,
// return any i such that arr[0] < arr[1] < ... arr[i - 1] < arr[i] > arr[i + 1] > ... > arr[arr.length - 1].

// Input: arr = [0,1,0]
// Output: 1

// Input: arr = [0,2,1,0]
// Output: 1

// Input: arr = [0,10,5,2]
// Output: 1

// Input: arr = [3,4,5,1]
// Output: 2

// Input: arr = [24,69,100,99,79,78,67,36,26,19]
// Output: 2

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/peak-index-in-a-mountain-array
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 也是二分查找; 不过找到的条件有点绕
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& arr) {
        int l = 0, r = arr.size() - 1;
        while (l < r) {
            int m = l + (r-l)/2;
            if (arr[m] > arr[m+1]) {
                r = m;    // m 比后面的要高; 顶峰可能是 m or 它的前面
            } else {
                l = m+1;  // m 比后面的要低; 顶峰一定在 m 后面
            }
        }
        return l;
    }
};