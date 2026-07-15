//std libs
#include <iostream>
#include <cstdlib>

// network libs
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


// my files
#include "Sockaddr.h"

void die(const char* msg) {
    std::cerr << "Error: " << msg << std::endl;
    exit(1);
}

int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv) {die("bind()");}


    std::cout << "Hello, world" << std::endl;

    return 0;
}