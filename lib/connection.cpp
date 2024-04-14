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
void Connection::echo(int sockfd) {
    char buf[BUF_LEN];
    while (true) {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0) {
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, bytes_read);
        } else if (bytes_read == -1 && errno == EINTR) {
            printf("continue reading");
            continue;
        } else if (bytes_read == -1
                   && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            printf("finish reading once, errno: %d\n", errno);
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