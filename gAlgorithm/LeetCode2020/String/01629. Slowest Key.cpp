/*
 * @lc app=leetcode id=1629 lang=cpp
 *
 * [1629] Slowest Key
 *
 * https://leetcode.com/problems/slowest-key/description/
 *
 * algorithms
 * Easy (59.07%)
 * Total Accepted:    19K
 * Total Submissions: 32.2K
 * Testcase Example:  '[9,29,49,50]\n"cbcd"'
 *
 * A newly designed keypad was tested, where a tester pressed a sequence of n
 * keys, one at a time.
 * 
 * You are given a string keysPressed of length n, where keysPressed[i] was the
 * i^th key pressed in the testing sequence, and a sorted list releaseTimes,
 * where releaseTimes[i] was the time the i^th key was released. Both arrays
 * are 0-indexed. The 0^th key was pressed at the time 0, and every subsequent
 * key was pressed at the exact time the previous key was released.
 * 
 * The tester wants to know the key of the keypress that had the longest
 * duration. The i^th^ keypress had a duration of releaseTimes[i] -
 * releaseTimes[i - 1], and the 0^th keypress had a duration of
 * releaseTimes[0].
 * 
 * Note that the same key could have been pressed multiple times during the
 * test, and these multiple presses of the same key may not have had the same
 * duration.
 * 
 * Return the key of the keypress that had the longest duration. If there are
 * multiple such keypresses, return the lexicographically largest key of the
 * keypresses.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: releaseTimes = [9,29,49,50], keysPressed = "cbcd"
 * Output: "c"
 * Explanation: The keypresses were as follows:
 * Keypress for 'c' had a duration of 9 (pressed at time 0 and released at time
 * 9).
 * Keypress for 'b' had a duration of 29 - 9 = 20 (pressed at time 9 right
 * after the release of the previous character and released at time 29).
 * Keypress for 'c' had a duration of 49 - 29 = 20 (pressed at time 29 right
 * after the release of the previous character and released at time 49).
 * Keypress for 'd' had a duration of 50 - 49 = 1 (pressed at time 49 right
 * after the release of the previous character and released at time 50).
 * The longest of these was the keypress for 'b' and the second keypress for
 * 'c', both with duration 20.
 * 'c' is lexicographically larger than 'b', so the answer is 'c'.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: releaseTimes = [12,23,36,46,62], keysPressed = "spuda"
 * Output: "a"
 * Explanation: The keypresses were as follows:
 * Keypress for 's' had a duration of 12.
 * Keypress for 'p' had a duration of 23 - 12 = 11.
 * Keypress for 'u' had a duration of 36 - 23 = 13.
 * Keypress for 'd' had a duration of 46 - 36 = 10.
 * Keypress for 'a' had a duration of 62 - 46 = 16.
 * The longest of these was the keypress for 'a' with duration 16.
 * 
 * 
 * Constraints:
 * 
 * 
 * releaseTimes.length == n
 * keysPressed.length == n
 * 2 <= n <= 1000
 * 1 <= releaseTimes[i] <= 10^9
 * releaseTimes[i] < releaseTimes[i+1]
 * keysPressed contains only lowercase English letters.
 * 
 * 
 */
class Solution {
public:
    char slowestKey(vector<int>& releaseTimes, string keysPressed) {
        // 1. 初始时; 最长时长 为 首个按下的键
        // 2. 从第二个键开始遍历
        // 3. 当前键的按下时长 == 当前释放 - 前置释放
        // 4. 如果当前健按下时间较长; 更新 键 和 最长时长
        // 5. 如果当前健按下时间 同于 最长时长; 更新 answer 为较大的键
        int longest = releaseTimes[0];       
        char answer = keysPressed[0];
        for (int i = 1; i < releaseTimes.size(); i++) {
            int press = releaseTimes[i] - releaseTimes[i-1];
            if (press > longest) {
                longest = press;
                answer = keysPressed[i];
            } else if (press == longest)
                answer = max(answer, keysPressed[i]);
        }
        return answer;
    }
};
