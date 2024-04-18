#include "threadpool.h"
#include <mutex>
ThreadPool::ThreadPool(int size) {
    for (int i = 0; i < size; i++) {
        threads.emplace_back(std::thread([this]() {
            while (1) {
                std::unique_lock<std::mutex> lock(tasks_mtx);
                cv.wait(lock, [this]() { return stop || !tasks.empty(); });
                if (stop || tasks.empty()) return;
                auto task = tasks.front();
                tasks.pop();
                lock.unlock();
                task();
            }
        }));
    }
}
ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    stop = 1;
    lock.unlock();
    cv.notify_all();
    for (auto &th : threads) {
        if (th.joinable()) th.join();
    }
}

void ThreadPool::add(std::function<void()> fun) {
    std::unique_lock<std::mutex> lock(tasks_mtx);
    tasks.emplace(fun);
    lock.unlock();
    cv.notify_one();
}