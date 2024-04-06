#include "util.h"
#include "log.h"
void setnoblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}
string recv(int fd) {
    char buf[1024];
    string res;
    while (true) {
        int len = read(fd, buf, sizeof(buf));
        if (len > 0) res += buf;
        if (len == -1 && errno == EINTR) continue;
        if (len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) break;
        if (len == 0) { // EOF，客户端断开连接
            printf("EOF, client fd %d disconnected\n", fd);
            close(fd); //关闭socket会自动将文件描述符从epoll树上移除
            break;
        }
    }
    cout << "server received :" << res << "\n";
    return res;
}
void sed(int fd, string s) {
    const char *p = s.c_str();
    int n = s.size();
    while (true) {
        int len = write(fd, p, n);
        err(len == -1, "write error!");
        if (len == n) break;
        p += len;
        n -= len;
    }
}