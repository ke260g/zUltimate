#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/fcntl.h>

int main( int argc, char **argv ) {
    int fd_fifo = open("trans_fd.fifo", O_WRONLY);
    int fd_fifo = open("trans_fd.fifo", O_WRONLY);
    return 0;
}
