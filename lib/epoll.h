#pragma once
#include "channel.h"
#include <sys/epoll.h>
#include <vector>
const int MAX_EVENTS = 1000;

class Epoll {
private:
    int epfd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    void updateChannel(Channel *);
    std::vector<Channel *> poll(int timeout = -1);
};