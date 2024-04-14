#include "acceptor.h"
#include "socket.h"
#include "iaddr.h"
#include "channel.h"
Acceptor::Acceptor(EventLoop *_loop) : loop(_loop) {
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
};
Acceptor::~Acceptor() {
    delete sock;
    delete addr;
    delete acceptChannel;
};
void Acceptor::acceptConnection() {
    InetAddress *clnt_addr = new InetAddress();
    Socket *clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(),
           inet_ntoa(clnt_addr->addr.sin_addr),
           ntohs(clnt_addr->addr.sin_port));
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}
void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> cb) {
    newConnectionCallback = cb;
}