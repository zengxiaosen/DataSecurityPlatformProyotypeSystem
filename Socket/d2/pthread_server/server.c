#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>

#include "wrap.h"

#define SERV_PORT 8000

void *do_work(void *arg)
{
    char buf[1024];
    int len, i;
    int cfd = (int)arg;

    pthread_detach(pthread_self());

    while (1) {
        len = Read(cfd, buf, sizeof(buf));
        if (len <= 0)
            break;
        Write(STDOUT_FILENO, buf, len);
        for (i = 0; i < len; ++i)
            buf[i] = toupper(buf[i]);
        Write(cfd, buf, len);
    }
    Close(cfd);
    return 0;
}

int main(int argc, char *argv[])
{
    int lfd, cfd;
    int serv_port = SERV_PORT;
    char client_ip[128];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len;
    pthread_t tid;

    if (argc == 2) 
        serv_port = atoi(argv[1]);

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((short)serv_port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 128);

    printf("wait for connect...\n");

    while (1) {
        client_len = sizeof(client_addr);
        cfd = Accept(lfd, (struct sockaddr *)&client_addr, &client_len);
        printf("client:%s\t%d\n",
                inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)),
                ntohs(client_addr.sin_port));

        pthread_create(&tid, NULL, do_work, (void *)cfd);
    }

    return 0;
}

