#include<cstring>
#include<cstdio>

class kmp2 {
    public:
        static int *gen_next(const char *s);
        static int kmp(const char *s, const char *p);
};

int *kmp2::gen_next(const char *p) {
    int *next = new int[strlen(p)];
    int i = 2,  // pathern      作为 字符串的索引
        j = 0; // pathern[0:j] 作为 模式传的
    next[0] = 0;
    next[1] = 0;
    while (i < (int)strlen(p)) {
        if (p[i-1] == p[j]) {
            ++i;
            ++j;
            next[i] = j;
        } else if (j == 0) { // 运行到这里; 说明 p[j = 0] ! s[i];
            next[i] = 0;
            ++i;             // 避免 kmp1 方法中上 j == -1 的额外循环
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

int kmp2::kmp(const char *s, const char *p) {
    int i = 0; // 字符串 s 的索引
    int j = 0; // 模式串 p 的索引
    int *next = gen_next(p);

    while (i < (int)strlen(s) && j < (int)strlen(p)) {
        if (s[i] == p[j]) {
            ++i;
            ++j;
        } else if (j == 0) { // 运行到这里; 说明 p[j = 0] ! s[i];
            ++i;             // 避免 kmp1 方法中上 j == -1 的额外循环
        } else
            j = next[j]; // mismatch: 索引 回溯到 PMT[j-1] 即 next[j] 处
        printf("i = %d, j = %d\n", i, j);
    }
    delete next;
    if (j == strlen(p))  // found 找到了
        return i - j;    // 返回 字符串中 模式串的起始指针
    else
        return -1;
}

int main( int argc, char **argv ) {
    int retval = kmp2::kmp("aaaabaaaabaaaabaaaaa", "aaaaa");
    printf("%d\n", retval);
    return 0;
}
