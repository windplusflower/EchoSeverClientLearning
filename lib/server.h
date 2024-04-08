#pragma once

#include "eventloop.h"
#include "socket.h"
class Server {
private:
    EventLoop *loop;
    Socket *serv_sock;

public:
    Server(EventLoop *);
    ~Server();

    void handleReadEvent(Socket *);
    void newConnection(Socket *serv_sock);
};