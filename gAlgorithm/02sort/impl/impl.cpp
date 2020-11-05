/**
 * @file 排序算法练习模板
 */

#include <vector>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
using namespace std;

bool valid_check(vector<int> &a, vector<int> &b) {
    if (b.size() != a.size()) return false;
    for (int i = 0; i < a.size(); ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

void build_array(vector<int> &a) {
    for (int j = 0; j < a.size(); ++j)
        a[j] = rand() % 200;
}

void print_array(vector<int> &a, const char *s) {
    printf("%10s", s);
    for (int j = 0; j < a.size(); ++j)
        printf("%5d ", a[j]);
    printf("\n");
}

class Sort {
private:
    int findmin(vector<int> &a) {
        if (a.empty()) return 0;
        int res = a[0];
        for (int i = 1; i < a.size(); ++i) {
            if (a[i] < res)
                res = a[i];
        }
        return res;
    }
    int findmax(vector<int> &a) {
        if (a.empty()) return 0;
        int res = a[0];
        for (int i = 1; i < a.size(); ++i) {
            if (a[i] > res)
                res = a[i];
        }
        return res;
    }
    /**
     * @exp: 从右往左 从一开始 的十进制位数
     */
    int findval(int a, int exp) {
        return (a / exp) % 10;
    }
    int findpivot(vector<int> &a, int head, int tail) {
        int mid = (tail - head) % 2 ?  (tail + head - 1)/2 : (tail + head) / 2;
        if ( (a[mid] - a[head])*(a[tail]-a[head]) < 0)
            return head;
        if ( (a[mid] - a[tail])*(a[head]-a[tail]) < 0)
            return tail;
        return mid;
    }
public:
    void sort(vector<int> &a) {
        for (int d = a.size()/2; d > 0; d /= 2) {
            for (int i = d; i < a.size(); i++) {
                int j = i;
                int pivot = a[i];
                for (j -= d; j >= 0; j -= d) {
                    if (a[j] > pivot)
                        a[j+d] = a[j];
                    else
                        break;
                }
                a[j+d] = pivot;
            }
        }
    }
};

int main( int argc, char **argv ) {
    Sort sorter;
    for (int i = 1; i <= 100; ++i) { // 检测 100 次
        // 1. 创建随机数组
        vector<int> a(i, 0);
        build_array(a);
        vector<int> b(a);

        // 2. 打印初始数组
        print_array(a, "before");

        // 3. 排序
        sorter.sort(a);

        // 4. 打印排序数组
        print_array(a, "after");

        // 5. 校验数组
        sort(b.begin(), b.end());

        // 6. 打印正确数组
        print_array(b, "answer");

        // 7. 校验结果
        printf("valid_check is %5s\n\n",valid_check(a, b) == true ? "true" : "false");
    }
    return 0;
}
