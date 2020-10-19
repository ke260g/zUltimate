// 匿名结构体的变量定义
static struct {
        int a;
        int b;
        int c;
} last_data, curr_data, *lp = &last_data, *cp = &curr_data;

int main(void) {
        curr_data.a = 100;
        curr_data.b = 300;
        curr_data.c = 600;
        *(&last_data) = *(&curr_data);
        printf("last a: %d, last b: %d, last c: %d\n", last_data.a, last_data.b, last_data.c);
        return 0;
}