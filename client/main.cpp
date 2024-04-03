#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "lib/log.h"
int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    err(sockfd == -1, "socket create error!");
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    while (connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr))) {
        continue;
    }
    while (true) {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        printf("enter message:");
        scanf("%s", buf);
        printf("\n");
        int len = write(sockfd, buf, strlen(buf));
        err(len == -1, "write error!");
        len = read(sockfd, buf, sizeof(buf));
        if (len > 0) {
            printf("recived %d bytes from sever:%s\n", len, buf);
        } else if (len == 0) {
            printf("disconnected!\n");
            close(sockfd);
            break;
        } else {
            printf("read error!");
            break;
        }
    }
    printf("SUCCESS\n");
}