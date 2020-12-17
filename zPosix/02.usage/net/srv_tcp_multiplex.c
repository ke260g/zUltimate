#include<sys/select.h>
#include<sys/poll.h>
#include<sys/epoll.h>

#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<stdio.h>

#define MAX_CLI 10
#define MAXLINE 1024
char buf[MAXLINE];

int get_srv_fd(int port, int max) {
    int listenfd;
    struct sockaddr_in addrsrv;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&addrsrv, sizeof(addrsrv));
    addrsrv.sin_family = AF_INET;
    addrsrv.sin_port = htons(port);
    addrsrv.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&addrsrv, sizeof(addrsrv));
    int retval = listen(listenfd, max);
    if (retval)
        return -1;
    return listenfd;
}

int echo_back(int clifd) {
    int n = 0;
    while(1) {
        n = read(clifd, buf, MAXLINE);
        if(n < 0) {
            if(errno == EINTR)
                continue;
        }
        break;
    }
    if (n > 0) {    // echo 回去
        buf[n] = '\0';
        printf("recv buf %s\n", buf);
        write(clifd, buf, strlen(buf) + 1);
    }
    return n;
}

void srv_select(int port);
void srv_poll(int port);
void srv_epoll(int port);

int main( int argc, char **argv ) {
    int port = 2333;
    // srv_select(port);
    // srv_poll(port);
    srv_epoll(port);
    return 0;
}

void srv_select(int port) {
    int i;
    int numfds, maxfd, srvfd, clifd;
    int clifds[MAX_CLI];
    fd_set readfds;
    struct timeval tv;
    struct sockaddr cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);

    memset(clifds, 0, sizeof(clifds));
    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = 30;


    srvfd = get_srv_fd(port, MAX_CLI);
    for(i = 0; i < MAX_CLI; ++i)
        clifds[i] = -1;

    while(1) {
        FD_ZERO(&readfds);
        FD_SET(srvfd, &readfds);

        // 更新maxfd
        maxfd = srvfd;
        // 加入 readfds
        for(i = 0; i < MAX_CLI; ++i) {
            clifd = clifds[i];
            if(clifd > 0) {
                FD_SET(clifd, &readfds);
                maxfd = maxfd > clifd ? maxfd : clifd;
            }
        }

        numfds = select(maxfd + 1, &readfds, NULL, NULL, &tv);
        if(numfds == -1) {
            if(errno == EINTR)
                continue;
            else
                ;
        }

        if(FD_ISSET(srvfd, &readfds)) {
            cliaddrlen = sizeof(cliaddr);
            clifd = accept(srvfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
            if(clifd == -1) {
                if(errno == EINTR)
                    continue;
            } else {
                // 加入 本地 clifd 缓存
                for(i = 0; i < MAX_CLI; ++i)
                    if(clifds[i] == -1)
                        clifds[i] = clifd;
                FD_SET(clifd, &readfds);
            }
        } else {
            for(i = 0; i < MAX_CLI; ++i) {
                clifd = clifds[i];
                if(clifd > 0 && FD_ISSET(clifd, &readfds)) {
                    if(echo_back(clifd) <= 0) { // 客户端关闭
                        FD_CLR(clifd, &readfds);
                        close(clifd);
                        clifds[i] = -1;
                    }
                }
            }
        }
    }
}

void srv_poll(int port) {
    struct sockaddr cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int i, numfds, srvfd, clifd, maxfd;
    struct pollfd clifds[MAX_CLI];

    // 初始化监听数组
    memset(clifds, 0, sizeof(clifds));
    for(i = 0; i < MAX_CLI; ++i)
        clifds[i].fd = -1;

    // listendfd
    srvfd = get_srv_fd(port, MAX_CLI);
    clifds[0].fd = srvfd;
    clifds[0].events = POLLIN;

    while(1) {
        // 更新maxfd
        maxfd = srvfd;
        for(i = 0; i < MAX_CLI; ++i) {
            maxfd = clifds[i].fd > maxfd ? clifds[i].fd : maxfd;
        }
        numfds = poll(clifds, maxfd + 1, -1);
        if(numfds == -1) {
            if(errno == EINTR)
                continue;
            else
                ;
        }

        if(clifds[0].revents & POLLIN)
        {
            cliaddrlen = sizeof(cliaddr);
            clifd = accept(srvfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
            if(clifd == -1)
            {
                if(errno == EINTR)
                    continue;
                else
                    ;

            }
            for(i = 1; i < MAX_CLI; ++i)
            {
                if(clifds[i].fd < 0)
                {
                    clifds[i].fd = clifd;
                    clifds[i].events = POLLIN;
                }
            }
        }
        for(i = 1; i < MAX_CLI; ++i) {
            if(clifds[i].fd > 0 && clifds[i].revents & POLLIN) {
                if(echo_back(clifds[i].fd) <= 0) {
                    close(clifds[i].fd);
                    clifds[i].fd = -1;
                }
            }
        }
    }
}

/////////////
// epoll demo
void epoll_add_event(int epfd, int fd, int event) {
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_del_event(int epfd, int fd, int event) {
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &ev);

}

void epoll_mod_event(int epfd, int fd, int event) {
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}

void srv_epoll(int port) {
    struct sockaddr cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);
    int i, numfds, srvfd, clifd, maxfd;
    int epfd;
    struct epoll_event evs[MAX_CLI];

    // 初始化监听数组

    // listendfd
    srvfd = get_srv_fd(port, MAX_CLI);

    // epoll创建
    epfd = epoll_create(MAX_CLI);
    // epoll添加
    epoll_add_event(epfd, srvfd, EPOLLIN);
    while(1) {
        // epoll等待
        numfds = epoll_wait(epfd, evs, MAX_CLI, -1);
        if(numfds < 0) {
            if(errno == EINTR)
                continue;
            else
                ;
        }
        for(i = 0; i < numfds; ++i)
        {
            if(srvfd == evs[i].data.fd) {
                if (!(evs[i].events & EPOLLIN)) {
                    continue;
                }
                while(1) {
                    clifd = accept(srvfd, (struct sockaddr *)&cliaddr, &cliaddrlen);
                    if(clifd < 0) {
                        if(errno == EINTR)
                            continue;
                        else
                            ;
                    } else {
                        epoll_add_event(epfd, clifd, EPOLLIN);
                    }
                    break;
                }
            }
            else {
                clifd = evs[i].data.fd;
                if(evs[i].events & EPOLLIN) {
                    if(echo_back(clifd) <= 0) { // 客户端关闭
                        close(clifd);
                        epoll_del_event(epfd, clifd, EPOLLIN);
                    }
                }
            }
        }
    }
}
