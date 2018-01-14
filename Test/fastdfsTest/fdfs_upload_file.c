#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "fdfs_client.h"

int upload_file2(const char* cfgfile, const char* myfile, char* fileID, int size){
    //父进程创建管道， 子进程也拥有这个管道
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1){
        perror("pipe error");
        exit(1);
    }

    //创建子进程
    pid_t pid = fork();
    if(pid == 0){
        //子进程
        //执行exec操作，写操作，关闭读端
        close(fd[0]);
        //重定向 - 标准输出 -> 管道的写端
        //new 跟随old， new重定向到old
        dup2(fd[1], STDOUT_FILENO);
        execlp("fdfs_upload_file", "fdfs_upload_file", cfgfile, myfile, NULL);
        perror("execlp error");
        exit(0);
    }

    //父进程
    else if(pid > 0){
        //读管道
        close(fd[1]);
        read(fd[0], fileID, size);
        close(fd[0]);
        //资源回收
        wait(NULL);
    }
}