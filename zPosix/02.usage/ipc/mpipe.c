#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<ftw.h>
#include<fcntl.h>
#include<unistd.h>

int main( int argc, char **argv ) {
    const char *cmdprev;
    const char *cmdnext;
    int i = 0, retval;
    int pipefd[2];
    retval = pipe(pipefd);

    pid_t pidprev, pidnext; // child


    pidprev = fork();
    if(pidprev == 0) {
        dup2(pipefd[1], STDOUT_FILENO);

        sleep(2);
        printf("a message from prev");

        exit(0);
    }

    pidnext = fork();
    if(pidnext == 0) {
        dup2(pipefd[0], STDIN_FILENO);

        char buf[1024];
        int n = read(STDIN_FILENO, buf, 1024);
        buf[n] = '\0';
        printf("I am next [%d]<%s>\n", n, buf);

        exit(0);
    }

    return 0;
}
