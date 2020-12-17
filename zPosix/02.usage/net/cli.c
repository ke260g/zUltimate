#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

#define MAXLINE 1024

void cli_echo(const char *message) {
    int sockfd, retval, n;
    char buf[MAXLINE];
    struct sockaddr_in srvaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&srvaddr, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(2333);
    inet_pton(AF_INET, "127.0.0.1", &srvaddr.sin_addr);

    retval = connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr));

    int i = 0;
    for(i = 0; i < 3; ++i) {
        write(sockfd, message, 32);
        n = read(sockfd, buf, MAXLINE);
        buf[n+1] = '\0';
        printf("%s\n", buf);
        sleep(1);
#if 0
        struct timespec ts;
        ts.tv_nsec = 500 * 1000;
        nanosleep(&ts, NULL);
#endif
    }
}

int main( int argc, char **argv ) {
    cli_echo("gaga");
    return 0;
}
