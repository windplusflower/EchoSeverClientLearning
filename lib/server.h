#pragma once

#include "eventloop.h"
#include "socket.h"
#include "acceptor.h"
class Server {
private:
    EventLoop *loop;
    Acceptor *acceptor;

public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(Socket *);
    void newConnection(Socket *serv_sock);
};