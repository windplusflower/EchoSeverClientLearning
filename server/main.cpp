#include "lib/util.h"
int main() {
    EventLoop eventloop;
    Server server(&eventloop);
    eventloop.loop();
    return 0;
}