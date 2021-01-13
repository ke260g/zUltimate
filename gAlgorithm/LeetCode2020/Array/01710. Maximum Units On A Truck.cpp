/*
 * @lc app=leetcode id=1710 lang=cpp
 *
 * [1710] Maximum Units on a Truck
 *
 * https://leetcode.com/problems/maximum-units-on-a-truck/description/
 *
 * algorithms
 * Easy (73.16%)
 * Total Accepted:    11.6K
 * Total Submissions: 16K
 * Testcase Example:  '[[1,3],[2,2],[3,1]]\n4'
 *
 * You are assigned to put some amount of boxes onto one truck. You are given a
 * 2D array boxTypes, where boxTypes[i] = [numberOfBoxesi,
 * numberOfUnitsPerBoxi]:
 * 
 * 
 * numberOfBoxesi is the number of boxes of type i.
 * numberOfUnitsPerBoxi is the number of units in each box of the type i.
 * 
 * 
 * You are also given an integer truckSize, which is the maximum number of
 * boxes that can be put on the truck. You can choose any boxes to put on the
 * truck as long as the number of boxes does not exceed truckSize.
 * 
 * Return the maximum total number of units that can be put on the truck.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: boxTypes = [[1,3],[2,2],[3,1]], truckSize = 4
 * Output: 8
 * Explanation: There are:
 * - 1 box of the first type that contains 3 units.
 * - 2 boxes of the second type that contain 2 units each.
 * - 3 boxes of the third type that contain 1 unit each.
 * You can take all the boxes of the first and second types, and one box of the
 * third type.
 * The total number of units will be = (1 * 3) + (2 * 2) + (1 * 1) = 8.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: boxTypes = [[5,10],[2,5],[4,7],[3,9]], truckSize = 10
 * Output: 91
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= boxTypes.length <= 1000
 * 1 <= numberOfBoxesi, numberOfUnitsPerBoxi <= 1000
 * 1 <= truckSize <= 10^6
 * 
 * 
 */
class Solution {
public:
    static bool cmpBox(vector<int> &x, vector<int> &y) {
        return x[1] > y[1];
    }
    int maximumUnits(vector<vector<int>>& boxTypes, int truckSize) {
        // 1. 根据 boxType 的 unit个数 排序; 升序
        // 2. 从左往右遍历 boxTypes
        // 3. 如果剩余 truckSize 不足以装满 当前 types 的所有 box
        //    那么只装 truckSize 个 (累计 units); 终止循环
        // 4. 否则把当前 types 的所有box 都装上 (累计 units)
        //    减去已经装上的 box 个数
        sort(boxTypes.begin(), boxTypes.end(),
             [](vector<int> &x, vector<int> &y) { return x[1] > y[1]; });
        int units = 0;
        for (auto box: boxTypes) {
            if (box[0] >= truckSize) {
                units += truckSize*box[1];
                break;
            } else
                units += box[0]*box[1];
            truckSize -= box[0];
        }
        return units;
    }
};
