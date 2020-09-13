#include<cstring>
#include<cstdio>

class kmp3 {
    public:
        static int *gen_next(const char *s);
        static int kmp(const char *s, const char *p);
};

int *kmp3::gen_next(const char *s) {
    int *next = new int[strlen(s)];
    next[0] = 0;  // 此处需要特殊处理
    next[1] = 0;  // PMT[0] 第一个元素没有 前后缀
    for (int i = 2; i < strlen(s); ++i) {
        next[i] = (s[i-1] == s[next[i-1]]) ? 
                    (s[i] == s[next[i-1]+1] ? next[next[i - 1]] : next[i-1] + 1 )
                    : 0;
    }
    for (int i = 0; i < strlen(s); ++i) {
        printf("%d ", next[i]);
    }
    printf("\n");
    return next;
}

int kmp3::kmp(const char *s, const char *p) {
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
    int retval = kmp3::kmp("aaaabaaaabaaaabaaaaa", "aaaaa");
    printf("%d\n", retval);
    return 0;
}
