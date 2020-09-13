#include<cstring>
#include<cstdio>

class kmp1 {
    public:
        static int *gen_next(const char *s);
        static int kmp(const char *s, const char *p);
};

// 求 next数组的过程; 其实就是一个kmp的过程
// 即 pathern作为 @字符串; 前缀pathern[0:j] 作为 @模式串 
int *kmp1::gen_next(const char *p) {
    int *next = new int[strlen(p)];
    int i = 0,  // pathern      作为 字符串的索引
        j = -1; // pathern[0:j] 作为 模式传的
    next[0] = -1; // 一个不存在的PMT项; 用来标记与 pathern首字符 mismatch

    while (i < (int)strlen(p)) {
        if (j == -1 || /* 跑到这里有两个场景: 1. 首次next[i=0];  */ 
                       /* 2. 上一次j = next[j = 0]; 即 p[i] != p[j=0] */
            p[i] == p[j]) {
            ++i;
            ++j;
            next[i] = j;
        } else {
            j = next[j]; // mismatch: 索引 回溯到 PMT[j-1] 即 next[j] 处
            // 注意: 这里 i不动; 表示 pathern不动; 仅右移 前缀pathern[0:j]
        }
            
    }

    for (int i = 0; i < strlen(p); ++i) {
        printf("%d ", next[i]);
    }
    printf("\n");
    return next;
}

int kmp1::kmp(const char *s, const char *p) {
    int i = 0; // 字符串 s 的索引
    int j = 0; // 模式串 p 的索引
    int *next = gen_next(p);

    while (i < (int)strlen(s) && j < (int)strlen(p)) {
        if (j == -1 || /* 跑到这里说明一次 j = next[j = 0]; 即s[i] != p[j = 0] */ 
            s[i] == p[j]) {
            ++i;
            ++j;
        } else
            j = next[j]; // mismatch: 索引 回溯到 PMT[j-1] 即 next[j] 处
            // 注意: 这里 i不动; 表示 string不动; 仅右移 pathern
        printf("i = %d, j = %d\n", i, j);
    }
    delete next;
    if (j == strlen(p))  // found 找到了
        return i - j;    // 返回 字符串中 模式串的起始指针
    else
        return -1;
        
}

int main( int argc, char **argv ) {
    int retval = kmp1::kmp("aaaabaaaabaaaabaaaaa", "aaaaa");
    printf("%d\n", retval);

    kmp1::gen_next("ababaabab");
    return 0;
}
