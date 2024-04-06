#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include "lib/log.h"
using std::string;
using std::cout;
const int MAX_EVENTS = 10;
void setnoblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
string recv(int fd) {
    char buf[1024];
    string res;
    while (true) {
        int len = read(fd, buf, sizeof(buf));
        if (len > 0) res += buf;
        if (len == -1 && errno == EINTR) continue;
        if (len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
        if (len == 0) { // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd); //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
    return res;
}
void sed(int fd, string s) {
    if (s.empty()) return;
    const char *p = s.c_str();
    int n = s.size();
    while (true) {
        int len = write(fd, p, n);
        err(len == -1, "write error!");
        if (len == n) break;
        p += len;
        n -= len;
    }
}
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
    err(listen(sockfd, SOMAXCONN) == -1, "listen error");
    int epollfd = epoll_create1(0);
    err(epollfd == -1, "epoll error");
    epoll_event events[MAX_EVENTS], ev;
    bzero(events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = sockfd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev);
    printf("start waiting\n");
    while (true) {
        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == sockfd) {
                sockaddr_in client_addr;
                bzero(&client_addr, sizeof(client_addr));
                socklen_t len = sizeof(client_addr);
                int clientfd = accept(sockfd, (sockaddr *)&client_addr, &len);
                err(clientfd == -1, "accept error");
                printf("new client fd %d! IP: %s Port: %d\n", clientfd,
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port));
                bzero(&ev, sizeof(ev));
                ev.data.fd = clientfd;
                ev.events = EPOLLIN | EPOLLET;
                setnoblocking(clientfd);
                epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
            } else if (events[i].events & EPOLLIN) {
                string message = recv(events[i].data.fd);
                if (!message.empty()) {
                    cout << "recived from " << events[i].data.fd << ' '
                         << message << "\n";
                    sed(events[i].data.fd, message);
                }
            }
        }
    }
    cout << "server closed";
}