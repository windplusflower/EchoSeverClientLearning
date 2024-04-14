#include "connection.h"
#include "channel.h"
#include "socket.h"
#include <unistd.h>
#include <strings.h>
Connection::Connection(EventLoop *_loop, Socket *_sock) :
    loop(_loop), sock(_sock) {
    channel = new Channel(loop, sock->getFd());
    channel->setCallback(std::bind(&Connection::echo, this, sock->getFd()));
    channel->enableReading();
}
Connection::~Connection() {
    delete channel;
    delete sock;
}
void Connection::echo(int fd) {
    if (fd == -1) {
        printf("socket fd invalid!\n");
        return;
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
            deleteConnectionCallback(sock);
            break;
        }
    }
    printf("received : %s\n", res.c_str());
    return;
}
void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> cb) {
    deleteConnectionCallback = cb;
}