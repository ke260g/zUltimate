/*
 * @lc app=leetcode id=804 lang=cpp
 *
 * [804] Unique Morse Code Words
 *
 * https://leetcode.com/problems/unique-morse-code-words/description/
 *
 * algorithms
 * Easy (78.77%)
 * Total Accepted:    172.2K
 * Total Submissions: 218.6K
 * Testcase Example:  '["gin", "zen", "gig", "msg"]'
 *
 * International Morse Code defines a standard encoding where each letter is
 * mapped to a series of dots and dashes, as follows: "a" maps to ".-", "b"
 * maps to "-...", "c" maps to "-.-.", and so on.
 * 
 * For convenience, the full table for the 26 letters of the English alphabet
 * is given below:
 * 
 * 
 * 
 * [".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."]
 * 
 * Now, given a list of words, each word can be written as a concatenation of
 * the Morse code of each letter. For example, "cab" can be written as
 * "-.-..--...", (which is the concatenation "-.-." + ".-" + "-..."). We'll
 * call such a concatenation, the transformation of a word.
 * 
 * Return the number of different transformations among all words we have.
 * 
 * 
 * Example:
 * Input: words = ["gin", "zen", "gig", "msg"]
 * Output: 2
 * Explanation: 
 * The transformation of each word is:
 * "gin" -> "--...-."
 * "zen" -> "--...-."
 * "gig" -> "--...--."
 * "msg" -> "--...--."
 * 
 * There are 2 different transformations, "--...-." and "--...--.".
 * 
 * 
 * Note:
 * 
 * 
 * The length of words will be at most 100.
 * Each words[i] will have length in range [1, 12].
 * words[i] will only consist of lowercase letters.
 * 
 * 
 */
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        // 1. 构造映射表; 返回字符串
        // 2. 遍历词组
        // 3. 针对每个词; 遍历每个字母; 把对应的morse表达; 拼接为字符串
        // 4. 把拼接出来的字符串放到集合中
        // 5. 最终返回集合的尺寸
        static const string morseAlpha[26] = {
            ".-", "-...", "-.-.", "-..", ".",
            "..-.", "--.", "....", "..", ".---",
            "-.-", ".-..", "--", "-.", "---",
            ".--.", "--.-", ".-.", "...", "-",
            "..-", "...-", ".--", "-..-", "-.--",
            "--.."
        };

        unordered_set<string> mset;
        for (auto word: words) {
            string curr;
            for (auto c: word)
                curr.append(morseAlpha[c-'a']);
            mset.insert(curr);
        }
        return mset.size();
    }
};
