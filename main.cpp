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


static void do_something(int connfd){
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if(n < 0){
        die("read() error");
        return;
    }

    std::cout << "client says: " << rbuf << std::endl;

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main(){
    // creating the socket
    // fd = handle you will use later
    int fd = socket(AF_INET, SOCK_STREAM, 0); 
    
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv) {die("bind()");}

    // listen
    rv = listen(fd, SOMAXCONN);
    if(rv) {die("listen()");}

    while(true){
        //accept
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);// client len

        int connfd = accept(fd,(struct sockaddr *)&client_addr, &addrlen);
        if(connfd < 0){
            continue;
        }

        do_something(connfd);
        close(connfd);
    }

    return 0;
}