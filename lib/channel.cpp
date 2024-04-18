#include "channel.h"
#include "eventloop.h"

Channel::Channel(EventLoop *_loop, int _fd) :
    loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false){};
Channel::~Channel(){};

void Channel::handleEvent() {
    loop->addThread(callback);
}
void Channel::enableReading() {
    events = EPOLLET | EPOLLIN;
    loop->updateChannel(this);
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

// void setEvents(uint32_t);
void Channel::setRevents(uint32_t _ev) {
    revents = _ev;
}
void Channel::setCallback(std::function<void()> _cb) {
    callback = _cb;
}