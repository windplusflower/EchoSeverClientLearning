#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "lib/log.h"
int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    err(sockfd == -1, "create socket error!");
    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);
    if (bind(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr))) {
        printf("bind error!");
        return 0;
    }
    listen(sockfd, SOMAXCONN);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, clnt_addr_len);
    int clnt_sockfd = accept(sockfd, (sockaddr *)&clnt_addr, &clnt_addr_len);
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd,
           inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    while (true) {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(clnt_sockfd, buf, sizeof(buf));
        if (len > 0) {
            printf("recevied %d bytes from client:%s\n", len, buf);
            write(clnt_sockfd, buf, len);
        } else if (len == 0) {
            printf("client %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else {
            err(1, "socket read error!\n");
        }
    }
}