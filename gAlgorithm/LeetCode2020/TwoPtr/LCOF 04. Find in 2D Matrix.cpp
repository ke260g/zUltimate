// 在一个 n * m 的二维数组中，
// 每一行都按照从左到右递增的顺序排序，
// 每一列都按照从上到下递增的顺序排序。
// 请完成一个函数，
// 输入这样的一个二维数组和一个整数
// 判断数组中是否含有该整数。

// [
//   [1,   4,  7, 11, 15],
//   [2,   5,  8, 12, 19],
//   [3,   6,  9, 16, 22],
//   [10, 13, 14, 17, 24],
//   [18, 21, 23, 26, 30]
// ]
// 给定 target = 5，返回 true。
// 给定 target = 20，返回 false。

// 来源：力扣（LeetCode）
// 链接：https://leetcode-cn.com/problems/er-wei-shu-zu-zhong-de-cha-zhao-lcof
// 著作权归领扣网络所有。商业转载请联系官方授权，非商业转载请注明出处。

// 经典双指针 头尾双指针问题: (双70% 自己写的)
// 头++; 尾部--;
// 分别找到符合条件的成员 matrix[head] or matrix[tail] 可能包含target
// 然后在可能包括的 vector 中进行二分查找
class Solution {
private:
    /** @brief 已经排序数组 a; 判断 t 可能在a 中*/
    bool maybeTargetInVector(vector<int> a, int t) {
        if (a.empty()) return false;
        return a[0] <= t && t <= a[a.size()-1];
    }
public:
    bool findNumberIn2DArray(vector<vector<int>>& matrix, int target) {
        int head = 0, tail = matrix.size()-1;
        while(head <= tail) {
            while (!maybeTargetInVector(matrix[head], target) && head < tail)
                head++;
            while (!maybeTargetInVector(matrix[tail], target) && head < tail)
                tail--;
            
            if (binary_search(matrix[head].begin(), matrix[head].end(), target))
                return true;
            else
                head++;
            if (binary_search(matrix[tail].begin(), matrix[tail].end(), target))
                return true;
            else
                tail--;
        }
        return false;
    }
};