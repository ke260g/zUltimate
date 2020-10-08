// Your friend is typing his name into a keyboard.  
// Sometimes, when typing a character c, the key might get long pressed, 
// and the character will be typed 1 or more times.

// You examine the typed characters of the keyboard.  
// Return True if it is possible that it was your friends name,
// with some characters (possibly none) being long pressed.

// Input: name = "alex", typed = "aaleex"
// Output: true
// Explanation: 'a' and 'e' in 'alex' were long pressed.

// Input: name = "saeed", typed = "ssaaedd"
// Output: false
// Explanation: 'e' must have been pressed twice, but it wasn't in the typed output.

// Input: name = "leelee", typed = "lleeelee"
// Output: true

// Input: name = "laiden", typed = "laiden"
// Output: true
// Explanation: It's not necessary to long press any character.

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/long-pressed-name
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 检查粘连字符串是否 匹配目标字符串
class Solution {
public:
    bool isLongPressedName(string name, string typed) {
        //  name: [0, j) 表示已经匹配的部分
        // typed: [0, i) 正在遍历的
        // 场景1.1: name[j] == typed[i] 匹配
        // 场景1.2: name[j] != typed[i] 但是 typed[i] == typed[i-1] 重复;
        // 场景1.3: name[j] != typed[i] 不匹配 且 不重复; 输入错误
        // 场景2: name 匹配还没结束 但typed 已经结束了
        // 场景3: name 已经匹配完成 说明 typed 前面部分输入匹配了name
        //       但是后部分输入可能不匹配
        //       因此 typed 后半部分 必须重复着name 的末尾字符
        int i = 0, j = 0;
        while (j < name.size() && i < typed.size()) {
            if (typed[i] == name[j]) { // 场景1.1
                j++;
                i++;
            } else if (i > 0 && typed[i] == typed[i-1]) {
                // 场景1.2
                i++;
            } else {
                // 场景1.3
                return false;
            }
        }

        if (j != name.size()) return false; // name 还没有匹配完

        // name 已经匹配完; typed 末尾必须重复着name的末尾
        while (i < typed.size()) {
            if (i > 0 && typed[i] == name[j-1])
                i++;
            else
                return false;
        }
        return true;
    }
};