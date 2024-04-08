#include "eventloop.h"

EventLoop::EventLoop() {
    ep = new Epoll();
    quit = false;
}
EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while (!quit) {
        auto events = ep->poll();
        for (auto channel : events) { channel->handleEvent(); }
    }
}
void EventLoop::updateChannel(Channel *channel) {
    ep->updateChannel(channel);
}