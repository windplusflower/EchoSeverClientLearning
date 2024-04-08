#pragma once
#include "epoll.h"
#include "channel.h"

class EventLoop {
private:
    Epoll *ep;
    bool quit;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *);
};