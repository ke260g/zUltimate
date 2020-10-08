// Write a function that takes a string as input and reverse only the vowels of a string.

// Input: "hello"
// Output: "holle"

// Input: "leetcode"
// Output: "leotcede"
// Explanation:
// leetcode
//  ^     ^ 交换
// leetcode
//   ^  ^   交换

// 形同快排的交换法
// 1. 两个指针；一个前一个后
// 2. 前指针++ (注意单次循环++ 也要判断终止条件 前指针<后指针)
// 3. 后指针-- (注意单次循环-- 也要判断终止条件 前指针<后指针)
// 4. 单次循环中; 符合条件就停止 ++ 和 --
// 5. 交换两个符合条件的 元素
//
// Note:
//   这个问题 待交换元素的条件是 都是元音(包括大小写)
//   而快速排序  待交换元素 a[tail] 的条件是 <  pivot
//               待交换元素 a[head] 的条件是 >= pivot
class Solution {
private:
    bool isVowel(char c) {
        char vowels[] = { 'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
        for (int i = 0; i < sizeof(vowels); ++i) {
            if (vowels[i] == c)
                return true;
        }
        return false;
    }
public:
    string reverseVowels(string s) {
        int head = 0, tail = s.size() - 1;
        while (head < tail) {
            while (!isVowel(s[head]) && head < tail)
                head++;
            while (!isVowel(s[tail]) && head < tail)
                tail--;
            swap(s[head++], s[tail--]);
        }
        return s;
    }
};