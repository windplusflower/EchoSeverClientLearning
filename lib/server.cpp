#include "server.h"
#include "acceptor.h"
#include "channel.h"
#include "iaddr.h"

Server::Server(EventLoop *_loop) : loop(_loop) {
    acceptor = new Acceptor(loop);
    acceptor->setNewConnectionCallback(
        std::bind(&Server::newConnection, this, std::placeholders::_1));
}
Server::~Server() {
    delete acceptor;
};

void Server::handleReadEvent(Socket *sock) {
    std::string s = sock->recv();
    printf("Received from fd %d:%s\n", sock->getFd(), s.c_str());
    sock->send(s);
}
void Server::newConnection(Socket *serv_sock) {
    InetAddress *clientaddr = new InetAddress;
    Socket *clientsock = new Socket(serv_sock->accept(clientaddr));
    printf("new client fd %d! IP: %s Port: %d\n", clientsock->getFd(),
           inet_ntoa(clientaddr->addr.sin_addr),
           ntohs(clientaddr->addr.sin_port));
    Channel *channel = new Channel(loop, clientsock->getFd());
    clientsock->setnonblocking();
    channel->setCallback(std::bind(&Server::handleReadEvent, this, clientsock));
    channel->enableReading();
}