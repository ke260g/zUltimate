#include<stdio.h>

struct MetaData {
    char *md_name;
    int   md_size;
    long  md_time;
};

struct MetaData gMetaData;

void *routine_beta(struct MetaData *p) {
    static int static_variable = 200;
    (p->md_size)--;
    return NULL;
}

void *routine_alpha(struct MetaData *p) {
    static int static_variable = 100;
    (p->md_size)++;
    routine_beta(p);
    return NULL;
}

void func(int *p[8]) {

}

int main(int argc, char **argv) {
    struct MetaData lMetaData = {
        .md_name = "local",
        .md_size = 5,
        .md_time = 100,
    };
    struct MetaData * p = &gMetaData;
    *p = lMetaData;

    routine_alpha(&lMetaData);

    int pA[8];
    int (*pB)[8];
    int *pC[8];

    for(int i = 0; i < sizeof(pA)/sizeof(pA[0]); ++i) {
        pA[i] = i;
    }
    pB = &pA;

    return 0;
}
