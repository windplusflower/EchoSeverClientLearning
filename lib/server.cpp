#include "server.h"
#include "channel.h"
#include "iaddr.h"

Server::Server(EventLoop *_loop) : loop(_loop) {
    serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    Channel *serv_chan = new Channel(loop, serv_sock->getFd());
    serv_chan->setCallback(std::bind(&Server::newConnection, this, serv_sock));
    serv_chan->enableReading();
}
Server::~Server() {
    delete serv_sock;
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