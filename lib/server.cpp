#include "server.h"
#include "acceptor.h"
#include "channel.h"

Server::Server(EventLoop *_loop) : loop(_loop) {
    acceptor = new Acceptor(loop);
    acceptor->setNewConnectionCallback(
        std::bind(&Server::newConnection, this, std::placeholders::_1));
}
Server::~Server() {
    delete acceptor;
};

void Server::newConnection(Socket *sock) {
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket *)> cb =
        std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
}
void Server::deleteConnection(Socket *sock) {
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}