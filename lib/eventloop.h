#pragma once
#include "epoll.h"
#include "channel.h"

class ThreadPool;
class EventLoop {
private:
    Epoll *ep;
    bool quit;
    ThreadPool *threadpool;

public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *);
    void addThread(std::function<void()>);
};