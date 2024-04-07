
#include <iostream>
#include "lib/iaddr.h"
#include "lib/util.h"
#include <map>
int main() {
    std::map<int, Socket *> sockets;
    Socket *server = new Socket();
    sockets[server->getFd()] = server;
    InetAddress *iaddr = new InetAddress("127.0.0.1", 8888);
    server->bind(iaddr);
    server->listen();
    Epoll *epoll = new Epoll();
    server->setnonblocking();
    epoll->addFd(server->getFd(), EPOLLET | EPOLLIN);
    while (true) {
        auto events = epoll->poll();
        for (auto v : events) {
            if (v.data.fd == server->getFd()) {
                InetAddress *clientaddr = new InetAddress();
                Socket *client = new Socket(server->accept(clientaddr));
                sockets[client->getFd()] = client;
                client->setnonblocking();
                epoll->addFd(client->getFd(), EPOLLIN | EPOLLET);
                printf("new client fd %d! IP: %s Port: %d\n", client->getFd(),
                       inet_ntoa(clientaddr->addr.sin_addr),
                       ntohs(clientaddr->addr.sin_port));
            } else if (v.events & EPOLLIN) {
                if (v.data.fd == -1) continue;
                Socket *client = sockets[v.data.fd];
                std::string message = client->recv();
                printf("received from sockfd %d :%s\n", v.data.fd,
                       message.c_str());
                if (client->getFd() != -1) client->send(message);
            }
        }
    }
    for (auto v : sockets) delete v.second;
}