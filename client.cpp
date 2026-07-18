//std libs
#include <iostream>
#include <cstdlib>
#include <cstring>  

// network libs for linux specificly
#include <sys/socket.h> // core socket funcs
#include <netinet/in.h> // structures for internet addreses
#include <arpa/inet.h> // IP address conversion ahelpers
#include <unistd.h> // general POSIX funcs


// my files
#include "Sockaddr.h"

void die(const char* msg) {
    std::cerr << "Error: " << msg << std::endl;
    exit(1);
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) die("socket()");

    mySockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
    
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv) die("connect");

    char msg[] = "hello";
    write(fd, msg, strlen(msg));
    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf ,sizeof(rbuf) - 1);
    if(n < 0) die("read");

    std::cout << "server says : " << rbuf << std::endl;

    return 0;
}