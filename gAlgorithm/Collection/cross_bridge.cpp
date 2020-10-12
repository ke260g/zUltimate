#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;
int cross_bridge(vector<int> & cost) {
    if (cost.empty()) return 0;
    sort(cost.begin(), cost.end());

    if (cost.size() == 1) return cost[0];
    if (cost.size() == 2) return cost[1]; // max(cost[0], cost[1]);

    int i = cost.size() - 1;
    int sum = 0;
    while (i >= 3) { // 至少要有4个人才能选方案
        // 过桥方案A
        // int planA = (max(cost[0], cost[i]) /* 一次 */) + (cost[0] /* 回城 */) + (max(cost[0], cost[i-1]) /* 第二次 */) + (cost[0] /* 回城 */);
        int planA = cost[i] + cost[i-1] + cost[0]*2;
        // 过桥方案B
        // int planB = (max(cost[0], cost[1]) /* 一次 */) + (cost[0] /* 回城 */) + (max(cost[i], cost[i-1]) /* 第二次 */) + (cost[1] /* 回城 */);
        int planB = cost[i] + cost[1]*2 + cost[0];
        sum += planA < planB ? planA : planB;
        i -= 2;
        cout << sum << endl;
    } // 前面的人过完桥后; 保证0, 1同时没有过桥
    if (i == 2) // 还剩下3个人
        // sum += max(cost[0], cost[1]) + cost[0] /* 回城 */ + max(cost[0], cost[2]); // 原始过桥计算
        sum += cost[1] + cost[0] + cost[2];

    else if (i == 1) // 只剩下最快的两个人
        // sum += max(cost[0], cost[1]); // 原始过桥计算
        sum += cost[1];
    return sum;

}
int main(void) {
        int a[] = {1,2,5,10};
        vector<int> cost(a, a+4);
        int sum = cross_bridge(cost);
        cout << sum << endl;
        return 0;
}