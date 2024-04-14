#pragma once

#include "eventloop.h"
#include "socket.h"
#include "acceptor.h"
#include "connection.h"
#include <map>

class Server {
private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection *> connections;

public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(Socket *);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};