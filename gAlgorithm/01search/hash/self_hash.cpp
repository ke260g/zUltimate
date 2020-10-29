/**
 * @file 自实现线性哈希
 *       不可以使用map; 可以使用 list, pair, vector
 *       为了简单起见, key仅支持int, value仅int
 */
#include<list>
#include<vector>
#include<utility>
#include<cstdio>
using namespace std;

class LinearHash {
private:
    vector<list<pair<int, int> *>> bucket;
    int size;
    int mod0; // 旧的取模
    int mod1; // 新的取模
    int expand_cursor;
    float loadFactor = 1; // 装载因子；可以为 0.75 / 0.8
    int hash0(int k) { return k % mod0; };
    int hash1(int k) { return k % mod1; };

    bool finish_expand() { return expand_cursor >= bucket.size()/2; }
    bool needed_expand() { return bucket.size()*loadFactor >= size; }
public:
    LinearHash() {
        mod0 = 1; // 初始 桶个数只有1
        mod1 = 2;
        bucket.resize(1);  // 初始 桶个数只有1
        size = 0; // 初始 没有元素加入
        expand_cursor = bucket.size();
    }
    void put(int k, int v) {
        // 1. 扩容
        if (needed_expand()) {
            bucket.resize(bucket.size()*2);
            expand_cursor = 0;
        }
        
        // 2. 取hash
        int h0 = hash0(k);

        // 3. 插入
        auto it = bucket[h0].begin();
        for (; it != bucket[h0].end(); it++) {
            if ((*it)->first == k)
                break;
        }
        // 3.1 分配新的节点
        if (it == bucket[h0].end()) {
            // 1. 创建节点
            pair<int, int> *node = new pair<int, int>;
            node->first = k;
            node->second = v;
            bucket[h0].push_back(node);
            size++;
        } else { // 3.2 替换旧的
            (*it)->second = v;
        }

        // 4. 扩容迁移
        if (expand_cursor < bucket.size()/2) { // 只迁移扩容后的前半部分
            auto it = bucket[expand_cursor].begin();
            for (; it != bucket[expand_cursor].end(); ++it) {
                int h1 = hash1((*it)->first); 
                if (h1 != expand_cursor) {
                    bucket[h1].push_back(*it);
                    it = bucket[expand_cursor].erase(it);
                    it--; // 注意迭代器的删除
                }
            }
            expand_cursor++;
            // 5. 完成扩容; 更新hash函数
            if (expand_cursor >= bucket.size()/2) {
                mod0 = mod1;
                mod1 *= 2;
            }
        }
    }
    int get(int k, int &v) {
        if (finish_expand()) // 已经扩展完成
            h = hash0(k);
        else { // 正在扩展
            h = hash1(k); // 先用新hash求值
            if (h < expand_cursor) // 该桶还没有完成扩展
                h = hash0(k); // 用回旧hash求值
        }
        for (auto it = bucket[h].begin(); it != bucket[h].end(); ++it) {
            if ((*it)->first == k) {
                v = (*it)->second;
                return 0;
            }
        }
        return -1;
    }
    void pop(int k) {
        if (finish_expand()) // 已经扩展完成
            h = hash0(k);
        else { // 正在扩展
            h = hash1(k); // 先用新hash求值
            if (h < expand_cursor) // 该桶还没有完成扩展
                h = hash0(k); // 用回旧hash求值
        }
        for (auto it = bucket[h].begin(); it != bucket[h].end(); ++it) {
            if ((*it)->first == k) {
                bucket[h].erase(it);
                return;
            }
        }    
    }
    void dump() {
        printf("size = %d, expand_cursor = %d, mod0 = %d, mod1 = %d\n",
            size,  expand_cursor, mod0, mod1);
        for (int i = 0; i < bucket.size(); ++i) {
            printf("[%4d]: ", i);
            for (auto it = bucket[i].begin(); it != bucket[i].end(); ++it) {
                printf("%4d ", (*it)->first);
            }
            printf("\n");
        }
        printf("\n");
    }
};

int main(void) {
    LinearHash linear_hash;
    int k[] = { 4, 8, 12, 5, 9, 6, 7, 11, 15, 19, 23 };
    for (int i = 0; i < sizeof(k)/sizeof(*k); ++i) {
        linear_hash.put(k[i], k[i]);
        linear_hash.dump();
    }
    return 0;
}