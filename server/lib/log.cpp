#include "log.h"
void err(bool sign, std::string message) {
    if (!sign) return;
    std::cout << message << std::endl;
    exit(-1);
}