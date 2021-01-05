/**
 * @file 多线程优雅退出; socketpair 二次挥手机制
 * 主线程           协线程
 * send(退出req)
 *                  recv(退出req)
 *                  销毁资源
 *                  send(退出ack)
 * recv(退出req)
 * pthread_join()
 * 销毁资源
 *
 *  代码中: 主线程创建了一个协线程
 *          主线程有定时器; 超时则发起退出请求
 *
 *          协线程定时任务打印自己的 tid
 *          接收到退出请求后则销毁资源
 *
 * @note 相互通知使用 socketpair (非阻塞)
 *       两个线程分别使用自己的 uv_loop (没有用 uv_loop 的方法)
 */

#include <uv.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int exitfd;
    int running;
    uv_pipe_t uv_pipe;
    uv_loop_t *loop;
    uv_timer_t timer;
} task_info_core_t;

typedef struct {
    int exitfd;
    pthread_t tid;
    uv_pipe_t uv_pipe;
    task_info_core_t *info_core;
    uv_timer_t gc_timer;
} task_info_main_t;

uv_loop_t *g_main_loop;
static int task_count;

static void alloc_cb_main(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    static char slab[1024];
    buf->base = slab;
    buf->len = sizeof(slab);
}

static void alloc_cb_core(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    static char slab[1024];
    buf->base = slab;
    buf->len = sizeof(slab);
}

// 协线程方法 ------------------------
// 销毁协线程的资源
static void task_exit_core_wrcb(uv_write_t *req, int status) {
    printf("[%8lu] core exit wrcb\n", pthread_self());

    task_info_core_t *info_core = (task_info_core_t *)req->data;
    info_core->running = 0;

}

static void task_exit_core_rdcb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    printf("[%8lu] core exit rdcb\n", pthread_self());
    uv_write_t write_req;
    write_req.data = stream->data;
    uv_write(&write_req, stream, buf, 1, task_exit_core_wrcb);// 回复主线程
}

static void task_run(uv_timer_t *timer) {
    printf("[%8lu] I am here.\n", pthread_self());
}

static void *task_loop(void *arg) {
    task_info_core_t *info_core = (task_info_core_t *)arg;

    uv_timer_init(info_core->loop, &info_core->timer);
    uv_timer_start(&info_core->timer, task_run, 0, 1000);

    info_core->running = 1;
    while (info_core->running)
        uv_run(info_core->loop, UV_RUN_ONCE);

    // 销毁 libuv 的资源; 需要再 run_once
    uv_timer_stop(&info_core->timer);
    uv_close((uv_handle_t *)&info_core->uv_pipe, NULL);
    uv_run(info_core->loop, UV_RUN_ONCE);

    // 最后销毁上层资源
    close(info_core->exitfd);
    uv_loop_close(info_core->loop);
    free(info_core->loop);
    free(info_core);
    return NULL;
}
// ----------------------------------

// 销毁主线程中协线程的资源
static void task_exit_main_rdcb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    task_info_main_t *info_main = (task_info_main_t *)stream->data;
    printf("[%8lu] main join task[%8lu]\n", pthread_self(), info_main->tid);

    pthread_join(info_main->tid, NULL);

    uv_close((uv_handle_t *)&info_main->uv_pipe, NULL);

    close(info_main->exitfd);
    free(info_main);
    task_count--;

    printf("[%8lu] main exit task\n", pthread_self());
}

static int nonblock(int fd) {
    int flag = fcntl(fd, F_GETFL);
    return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

static void task_gc(uv_timer_t *timer) {
    printf("[%8lu] main gc task\n", pthread_self());
    task_info_main_t * info_main = (task_info_main_t *)timer->data;

    char c[1];
    uv_buf_t buf;
    buf.base = c;
    buf.len = 1;
    uv_write_t write_req;
    uv_write(&write_req, (uv_stream_t *)&info_main->uv_pipe, &buf, 1, NULL);
}

static int task_new() {
    // 创建 socketpair; 退出时二次挥手
    int pairfd[2];
    socketpair(AF_UNIX, SOCK_DGRAM, 0, pairfd);
    nonblock(pairfd[0]);
    nonblock(pairfd[1]);

    // 主线程中的句柄
    task_info_main_t *info_main = (task_info_main_t *)malloc(sizeof(task_info_main_t));
    info_main->exitfd = pairfd[0];
    uv_pipe_init(g_main_loop, &info_main->uv_pipe, 0);
    info_main->uv_pipe.data = info_main;
    uv_pipe_open(&info_main->uv_pipe, info_main->exitfd);
    uv_read_start((uv_stream_t*)&info_main->uv_pipe, alloc_cb_main, task_exit_main_rdcb);

    // 协线程中的句柄
    task_info_core_t *info_core = (task_info_core_t *)malloc(sizeof(task_info_core_t));
    info_core->exitfd = pairfd[1];
    info_core->loop = uv_loop_new();
    uv_pipe_init(info_core->loop, &info_core->uv_pipe, 0);
    info_core->uv_pipe.data = info_core;
    uv_pipe_open(&info_core->uv_pipe, info_core->exitfd); // fd 由上层进程创建
    uv_read_start((uv_stream_t*)&info_core->uv_pipe, alloc_cb_core, task_exit_core_rdcb);

    info_main->info_core = info_core;

    // 超时回收
    uv_timer_init(g_main_loop, &info_main->gc_timer);
    info_main->gc_timer.data = info_main;
    uv_timer_start(&info_main->gc_timer, task_gc, 3*1000, 0);

    // 全局容器记录
    task_count++;

    pthread_t tid;
    pthread_create(&tid, NULL, task_loop, info_core);
    info_main->tid = tid;

    return 0;
}

int main( int argc, char **argv ) {
    g_main_loop = uv_default_loop();
    printf("[%8lu] I am main.\n", pthread_self());

    // 创建线程
    task_new();

    // 运行
    while (task_count > 0)
        uv_run(g_main_loop, UV_RUN_ONCE);

    printf("[%8lu] finish all\n", pthread_self());
    return 0;
}
