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
// -------------------

// 环形数组的 左移 右移
int ring_buffer[1024];
int curr_cursor;
void ring_buff_forward(int step) {
    curr_cursor =  (curr_cursor + step) % array_count(ring_buff);
}
void ring_buff_backward(int step) {
    // 逆向步进 = 步进一圈 再反向走
    // 因为直接方向走 会 导致索引变为负数; 负数取模的出现负数索引
    curr_cursor = (curr_cursor + array_count(ring_buff) - step) % array_count(ring_buff);
}
// -------------------