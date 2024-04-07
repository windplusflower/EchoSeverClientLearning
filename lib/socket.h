#pragma once
#include <string>

const int BUF_LEN = 1024;

class InetAddress;
class Socket {
private:
    int fd;

public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddress *);
    void listen();
    void setnonblocking();

    int accept(InetAddress *);

    int getFd();
    std::string recv();
    void send(std::string);
};