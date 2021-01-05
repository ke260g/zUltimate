// You have an integer and you can flip exactly one bit from a 0 to a 1.
// Write code to find the length of the longest sequence of 1s you could create.

// Input: num = 1775(110111011112)
// Output: 8

// Input: num = 7(01112)
// Output: 4

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/reverse-bits-lcci
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

/**
 * @brief  实际上是 以0分割  `1-0`字符串
 *         找到 0 左右两边连续的1加起来较多的值
 *         
 *         可以认为 int 是一个 32 个字符的字符串
 *         并且最低位的右边补上一个0
 *         
 *         使用形同动态规划的思路
 *         记录上一个0的左边1个数 和 上一个0的右边1的个数
 *         假设最低位的右边补上一个0; 那么最先遇到的1, 就是上一个0的左边的1个数++
 *         继续往前
 *         如果遇到1; 那么 上一个0的左边1个数++
 *         如果遇到0; 那么 当前0变成了 "上一个0"
 *                    因此 上一个0的右边1个数 = 上一个0左边1个数
 *                    并且 上一个0的左边1个数 = 0 (没有了)
 *         遍历每一位后; 
 *             最大长度 = max(最大长度, 上一个0的左边1个数 + 上一个0的右边1个数 + 1)
 */
class Solution {
public:
    int reverseBits(int num) {
        if (num == -1) return 32;
        int l = 0, r = 0, ans = 0;
        for (int i = 0; i < 32; ++i) {
            if (num & 0x1) {
                l++;
            } else {
                r = l;
                l = 0;
            }
            ans = max(ans, l+r+1);
            num >>= 1;
        }
        return ans;
    }
};