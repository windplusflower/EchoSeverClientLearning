#include "channel.h"
#include "eventloop.h"

Channel::Channel(EventLoop *_loop, int _fd) :
    loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false),
    usepool(false){};
Channel::~Channel(){};

void Channel::handleEvent() {
    if (usepool)
        loop->addThread(callback);
    else
        callback();
}
void Channel::enableReading() {
    events |= EPOLLPRI | EPOLLIN;
    loop->updateChannel(this);
}
void Channel::useET() {
    events |= EPOLLET;
}

int Channel::getFd() {
    return fd;
}
uint32_t Channel::getEvents() {
    return events;
}
uint32_t Channel::getRevents() {
    return revents;
}
bool Channel::getInEpoll() {
    return inEpoll;
}
void Channel::setInEpoll() {
    inEpoll = 1;
}
void Channel::useThreadpool() {
    usepool = 1;
}

// void setEvents(uint32_t);
void Channel::setRevents(uint32_t _ev) {
    revents = _ev;
}
void Channel::setCallback(std::function<void()> _cb) {
    callback = _cb;
}