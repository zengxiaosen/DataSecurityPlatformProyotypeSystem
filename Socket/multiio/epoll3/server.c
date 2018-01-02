#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include "wrap.h"
#define MAXLINE 1024
#define SERV_PORT 8000
#define OPEN_MAX 10000

int efd;
struct do_work {
    int fd;
    void (*fun)(void *);
    void *arg;
};
void do_client(void *arg)
{
    int n, i, j, res;
    char buf[MAXLINE];
    struct do_work *dowork = (struct do_work *)arg;

    n = Read(dowork->fd, buf, MAXLINE);
    if (n == 0) {
        res = epoll_ctl(efd, EPOLL_CTL_DEL, dowork->fd, NULL);
        if (res == -1)
            perr_exit("epoll_ctl");
        Close(dowork->fd);
        printf("client[%d] closed connection\n", j);
    }
    else if (n < 0)
        perr_exit("read");
    else {
        for (j = 0; j < n; j++)
            buf[j] = toupper(buf[j]);
        Writen(dowork->fd, buf, n);
    }

}
int main(int argc, char *argv[])
{
    int i, j, maxi, listenfd, connfd, sockfd;
    int  n;
    ssize_t nready, res;
    char buf[MAXLINE], str[INET_ADDRSTRLEN];
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    struct epoll_event tep, ep[OPEN_MAX];
    struct do_work work;

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    Listen(listenfd, 20);

    efd = epoll_create(OPEN_MAX);
    if (efd == -1)
        perr_exit("epoll_create");

    tep.events = EPOLLIN; tep.data.fd = listenfd;

    res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);
    if (res == -1)
        perr_exit("epoll_ctl");
    for ( ; ; ) {
        nready = epoll_wait(efd, ep, OPEN_MAX, -1);
        /* 阻塞监听 */
        if (nready == -1)
            perr_exit("epoll_wait");
        for (i = 0; i < nready; i++) {
            if (!(ep[i].events & EPOLLIN))
                continue;
            if (ep[i].data.fd == listenfd) {
                clilen = sizeof(cliaddr);
                connfd = Accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
                printf("received from %s at PORT %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)), ntohs(cliaddr.sin_port));

                tep.events = EPOLLIN; 
                work.fd = connfd;
                work.fun = do_client;
                work.arg = &work;
                tep.data.ptr= &work;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
                if (res == -1)
                    perr_exit("epoll_ctl");
            }
            else {
                do_client(ep[i].data.ptr);
            }
        }
    }
    close(listenfd);
    close(efd);
    return 0;
}

