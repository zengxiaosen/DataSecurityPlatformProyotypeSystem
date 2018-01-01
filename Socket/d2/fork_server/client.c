#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

#define SERV_PORT 8000

int main(int argc, char *argv[])
{
    int sfd, len;
    struct sockaddr_in serv_addr;
    char buf[4096];

    if (argc < 2) {
        printf("./client serv_ip\n");
        return 1;
    }

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);

    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (fgets(buf, sizeof(buf), stdin)) {
        write(sfd, buf, strlen(buf));
        len = read(sfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
    }
    return 0;
}
