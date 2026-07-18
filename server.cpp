#include "utilities.h"

int main(){
    // creating the socket
    // fd = handle you will use later
    int fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(fd < 0)die("fd");

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    mySockaddr_in addr = {};
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
        struct mySockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);// client len

        int connfd = accept(fd,(struct sockaddr *)&client_addr, &addrlen);
        if(connfd < 0){
            continue;
        }

        while(true){
            int32_t err = one_request(connfd); // in utilities.h
            if(err < 0) break;
        }

        //do_something(connfd);
        close(connfd);
    }

    return 0;
}