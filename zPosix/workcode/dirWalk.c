#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<limits.h>
#include<ftw.h>

void dirWalkDeepFirst(const char *path) {
    // 继承路径前缀
    char *npath = (char *)malloc(strlen(path) + 1 + NAME_MAX + 1);
    strcpy(npath, path);
    npath[strlen(path)] = '/';

    // 打开当前目录
    DIR *dirp = opendir(path);
    struct dirent * direntp;
    struct stat mstat;
    printf("%s\n", npath);
    while(direntp = readdir(dirp))
    {   // 遍历
        const char * dname = direntp->d_name;
        if(!strcmp(dname, "..") || !strcmp(dname, "."))
            continue;

        // 子路径
        strcpy(npath + strlen(path) + 1, dname);

        stat(npath, &mstat);
        if( (mstat.st_mode & S_IFMT) == S_IFDIR)
        {
            // 目录
            dirWalkDeepFirst(npath);
        }
        else // 文件
            printf("%s\n", npath);
    }

    closedir(dirp);
    free(npath);
}

int show_name(const char *fpath, const struct stat *sb, int typeflag) {
    printf("%s\n", fpath);
    return 0;
}

int main( int argc, char **argv ) {
    // dirWalkDeepFirst("/usr/share/X11");
    const char * path = "../../zDarkTree";
    dirWalkDeepFirst(path);
    ftw(path, show_name, 20);
    return 0;
}
