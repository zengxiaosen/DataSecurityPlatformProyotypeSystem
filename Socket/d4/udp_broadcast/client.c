#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#define SERVER_PORT 8000
#define MAXLINE 4096

#define CLIENT_PORT 9000
int main(int argc, char *argv[])
{
    struct sockaddr_in serveraddr, localaddr;
    int confd;
    ssize_t len;
    char buf[MAXLINE];

    //1.创建一个socket
    confd = socket(AF_INET, SOCK_DGRAM, 0);

    //2.初始化本地端地址
    bzero(&localaddr, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0" , &localaddr.sin_addr.s_addr);
    localaddr.sin_port = htons(CLIENT_PORT);

    bind(confd, (struct sockaddr *)&localaddr, sizeof(localaddr));

    socklen_t slen;
    while (1) {
        slen = sizeof(serveraddr);
        len = recvfrom(confd, buf, sizeof(buf), 0, (struct sockaddr *)&serveraddr, &slen);
        printf("%s", buf);
        //printf("%s\n", inet_ntop(AF_INET, &serveraddr.sin_addr.s_addr, buf, sizeof(buf)));
        //write(STDOUT_FILENO, buf, len);
    }
    close(confd);
    return 0;
}

