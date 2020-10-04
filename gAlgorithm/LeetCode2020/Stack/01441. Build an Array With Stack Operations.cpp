// 给你一个目标数组 target 和一个整数 n。每次迭代，需要从  list = {1,2,3..., n} 中依序读取一个数字。
// 请使用下述操作来构建目标数组 target ：
//     Push：从 list 中读取一个新元素， 并将其推入数组中。
//     Pop：删除数组中的最后一个元素。
//     如果目标数组构建完成，就停止读取更多元素。
// 题目数据保证目标数组严格递增，并且只包含 1 到 n 之间的数字。
// 请返回构建目标数组所用的操作序列。
// 题目数据保证答案是唯一的。

// 输入：target = [1,3], n = 3
// 输出：["Push","Push","Pop","Push"]
// 解释： 
// 读取 1 并自动推入数组 -> [1] ["Push"]
// 读取 2 并自动推入数组，不符合条件, 然后删除它 -> [1] ["Push", "Push", "Pop"]
// 读取 3 并自动推入数组 -> [1,3] ["Push","Push","Pop","Push"]

// 输入：target = [1,2], n = 4
// 输出：["Push","Push"]
// 解释：只需要读取前 2 个数字就可以停止。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/build-an-array-with-stack-operations
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// // 给定目标数组; 求出构造该数组的栈操作
class Solution {
public:
    vector<string> buildArray(vector<int>& target, int n) {
        // 1. 先把目标数组target 反序构造成栈stk
        // 2. 从1 ~ n 开始输入
        // 3.1 每个数字先 进行Push操作
        // 3.2 如果等同 stk; 则 stk弹出
        // 3.3 否则      进行Pop 操作
        stack<int> stk;
        for (auto it = target.rbegin(); it != target.rend(); ++it)
            stk.push(*it); 
        vector<string> s;
        for (int i = 1; i <= *target.rbegin(); ++i) {
            s.push_back("Push");
            if (i == stk.top()) { // 符合条件
                stk.pop();
            } else {              // 不符合
                s.push_back("Pop");
            }
        }
        return s;
    }
};