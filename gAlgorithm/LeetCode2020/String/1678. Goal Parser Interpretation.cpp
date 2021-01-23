/*
 * @lc app=leetcode id=1678 lang=cpp
 *
 * [1678] Goal Parser Interpretation
 *
 * https://leetcode.com/problems/goal-parser-interpretation/description/
 *
 * algorithms
 * Easy (86.98%)
 * Total Accepted:    27.2K
 * Total Submissions: 31.5K
 * Testcase Example:  '"G()(al)"'
 *
 * You own a Goal Parser that can interpret a string command. The command
 * consists of an alphabet of "G", "()" and/or "(al)" in some order. The Goal
 * Parser will interpret "G" as the string "G", "()" as the string "o", and
 * "(al)" as the string "al". The interpreted strings are then concatenated in
 * the original order.
 * 
 * Given the string command, return the Goal Parser's interpretation of
 * command.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: command = "G()(al)"
 * Output: "Goal"
 * Explanation: The Goal Parser interprets the command as follows:
 * G -> G
 * () -> o
 * (al) -> al
 * The final concatenated result is "Goal".
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: command = "G()()()()(al)"
 * Output: "Gooooal"
 * 
 * 
 * Example 3:
 * 
 * 
 * Input: command = "(al)G(al)()()G"
 * Output: "alGalooG"
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= command.length <= 100
 * command consists of "G", "()", and/or "(al)" in some order.
 * 
 * 
 */
class Solution {
public:
    string interpret(string command) {
        string answer;
        // Gal 直接转
        // ')' 判断前置是否是 '(', 如果是则转义为 o
        for (int i = 0; i < command.size(); ++i) {
            if (command[i] == 'G' || command[i] == 'a' || command[i] == 'l')
                answer.append(1, command[i]);
            else if (command[i] == ')' && command[i-1] == '(')
                answer.append(1, 'o');
        }
        return answer;
    }
};
