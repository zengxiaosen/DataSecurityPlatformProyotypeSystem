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
void do_sig(int num){
    while(waitpid(0, NULL, WNOHANG) > 0)
    ;
}

int main(void){
    int lfd, cfd, len, i;
    int serv_port = SERV_PORT;
    char buf[1024], client_ip[128];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_len;
    pid_t pid;
    struct sigaction act;

    act.sa_handler = do_sig;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, NULL);

    if(argc == 2){
        serv_port = atoi(argv[1]);
    }
    lfd = Socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons((short)serv_port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 128);

    printf("wait for connect...\n");

    while(1){
        client_len = sizeof(client_addr);
        cfd = Accept(lfd, (struct sockaddr *)&client_addr, &client_len);
        printf("client:%s\t%d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client_addr.sin_port));
        pid = fork();
        if(pid == 0){
            // in child
            Close(lfd);
            while(1){
                len = Read(cfd, buf, sizeof(buf));
                if(len <= 0){
                    break;
                }
                Write(STDOUT_FILENO, buf, len);
                for(i=0; i< len; ++i){
                    buf[i] = toupper(buf[i]);
                }
                Write(cfd, buf, len);
            }
            Close(cfd);
            return 0;
        }else if (pid > 0){
            // in parent
            Close(cfd);
        }else{
            perror("fork");
            exit(1);
        }
    }
    Close(lfd);
    return 0;


}