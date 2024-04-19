#include "socket.h"
#include "log.h"
#include "iaddr.h"
#include <sys/socket.h>
#include <fcntl.h>
Socket::Socket() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    err(fd == -1, "error when create socket!");
}
Socket::Socket(int fd) : fd(fd) {
    err(fd == -1, "error when create socket!");
}
Socket::~Socket() {
    if (fd != -1) close(fd);
    fd = -1;
}

void Socket::bind(InetAddress *iaddr) {
    err(::bind(fd, (sockaddr *)&iaddr->addr, iaddr->addr_len) == -1,
        "error when bind socket");
}
void Socket::listen() {
    err(::listen(fd, SOMAXCONN) == -1, "error when listen socket");
}
void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *iaddr) {
    int clientfd = ::accept(fd, (sockaddr *)&iaddr->addr, &iaddr->addr_len);
    err(clientfd == -1, "error when accept socket");
    return clientfd;
}

int Socket::getFd() {
    return fd;
}

std::string Socket::recv() {
    if (fd == -1) {
        printf("socket fd invalid!\n");
        return "";
    }
    char buf[BUF_LEN];
    bzero(buf, BUF_LEN);
    std::string res = "";
    while (true) {
        int len = read(fd, buf, BUF_LEN);
        if (len > 0) res += buf;
        if (len == -1 && errno == EINTR) continue;
        if (len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
        if (len == 0) {
            printf("client %d closed\n", fd);
            close(fd);
            fd = -1;
            break;
        }
    }
    return res;
}
void Socket::send(std::string message) {
    if (fd == -1) {
        printf("socket fd invalid!\n");
        return;
    }
    if (message.empty()) return;
    const char *p = message.c_str();
    int n = message.size();
    while (true) {
        int len = write(fd, p, n);
        err(len == -1, "write error!");
        if (len == n) break;
        p += len;
        n -= len;
    }
}
void Socket::connect(InetAddress *_addr) {
    struct sockaddr_in addr = _addr->addr;
    err(::connect(fd, (sockaddr *)&addr, sizeof(addr)) == -1,
        "socket connect error");
}