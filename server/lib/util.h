#include <string>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

const int MAX_EVENTS = 10;
const int READ_BUFFER = 10;
const int WRITE_BUFFER = 10;

using std::string;
using std::cin;
using std::cout;

string recv(int);
void sed(int, std::string);
void setnoblocking(int);