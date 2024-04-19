#include "connection.h"
#include "channel.h"
#include "socket.h"
#include "buffer.h"
#include "log.h"
#include <unistd.h>
#include <strings.h>
Connection::Connection(EventLoop *_loop, Socket *_sock) :
    loop(_loop), sock(_sock) {
    channel = new Channel(loop, sock->getFd());
    channel->setCallback(std::bind(&Connection::echo, this, sock->getFd()));
    channel->useET();
    channel->useThreadpool();
    channel->enableReading();
}
Connection::~Connection() {
    delete channel;
    delete sock;
}
void Connection::echo(int sockfd) {
    char buf[BUF_LEN];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            buffer.append(buf);
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading");
            continue;
        } else if (bytes_read == -1
                   && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once\n");
            printf("message from client fd %d: %s\n", sockfd, buffer.c_str());
            err(write(sockfd, buffer.c_str(), buffer.size()) == -1,
                "socket write error");
            buffer.clear();
            break;
        } else if (bytes_read == 0) {
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sock);
            break;
        }
    }
}
void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> cb) {
    deleteConnectionCallback = cb;
}