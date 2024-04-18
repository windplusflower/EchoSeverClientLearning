#include "eventloop.h"
#include "threadpool.h"

EventLoop::EventLoop() {
    ep = new Epoll();
    threadpool = new ThreadPool;
    quit = false;
}
EventLoop::~EventLoop() {
    delete ep;
    delete threadpool;
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
void EventLoop::addThread(std::function<void()> func) {
    threadpool->add(func);
}