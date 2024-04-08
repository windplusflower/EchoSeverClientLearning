#include "epoll.h"
#include "log.h"
#include <sys/epoll.h>
#include <unistd.h>

Epoll::Epoll() : epfd(-1), events(nullptr) {
    epfd = epoll_create1(0);
    err(epfd == -1, "create epoll failed!\n");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}
Epoll::~Epoll() {
    if (epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

void Epoll::addFd(int fd, uint32_t op) {
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = op;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}
std::vector<Channel *> Epoll::poll(int timeout) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    err(nfds == -1, "error when epoll_wait!");
    std::vector<Channel *> res;
    for (int i = 0; i < nfds; i++) {
        Channel *ch = (Channel *)events[i].data.ptr;
        ch->setRevents(events[i].events);
        res.push_back(ch);
    }
    return res;
}

void Epoll::updateChannel(Channel *channel) {
    int fd = channel->getFd();
    epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if (!channel->getInEpoll()) {
        err(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1,
            "error when add channel to epoll");
        channel->setInEpoll();
    } else {
        err(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1,
            "error when modify channel in epoll");
    }
}