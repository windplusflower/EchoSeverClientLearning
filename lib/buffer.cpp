#include "buffer.h"
#include <iostream>
void Buffer::append(const char *_str) {
    buf += _str;
}
ssize_t Buffer::size() {
    return buf.size();
}
const char *Buffer::c_str() {
    return buf.c_str();
}
void Buffer::clear() {
    buf = "";
}
void Buffer::getline() {
    std::getline(std::cin, buf);
}