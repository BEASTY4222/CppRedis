//std libs
#include <iostream>
#include <cstdlib>
#include <cstring>  
#include <cassert>
#include <cerrno>

// network libs for linux specificly
#include <sys/socket.h> // core socket funcs
#include <netinet/in.h> // structures for internet addreses
#include <arpa/inet.h> // IP address conversion ahelpers
#include <unistd.h> // general POSIX funcs


// my files
#include "Sockaddr.h"
const size_t k_max_msg = 4096;


static void msg(std::string str){
    std::cout << str << std::endl;
}
void die(const char* msg) {
    std::cerr << "Error: " << msg << " (" << strerror(errno) << ")" << std::endl;    exit(1);
}

static int32_t read_full(int fd, char *buf, size_t n){
    while(n > 0){
        ssize_t rv = read(fd, buf, n);
        if(rv <= 0) return -1;//error or unexpected EOF

        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }

    return 0;
}

static int32_t write_all(int fd, const char *buffer, size_t n){
    while(n > 0){
        ssize_t rv = write(fd, buffer, n);
        if(rv <= 0) return -1;// error
        assert((size_t)rv <= 4);
        n-= (size_t)rv;
        buffer += rv;
    }
    return 0;
}

static int32_t one_request(int connfd){
    // 4 bytes header
    char readBuffer[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(connfd, readBuffer, 4);
    if(err < 0) {
        die(errno == 0 ? "EOF" : "read() error");
        return errno;
    }

    uint32_t len = 0;
    memcpy(&len, readBuffer, 4);// little endian big byte first
    if(len > k_max_msg){
        msg("too long");
        return  -1;
    }

    //request body
    err = read_full(connfd, &readBuffer[4], len);
    if(err){
        die("read() error");
        return err;
    }

    // do something
    std::cout << "client says: "<< len << " " << &readBuffer[4] << std::endl;
    const char reply[] = "world";
    char writeBuffer[4 + sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(writeBuffer, &len, 4);
    memcpy(&writeBuffer[4], reply, len);
    
    return write_all(connfd, writeBuffer, 4 + len);
}




static void do_something(int connfd){
    char readbuf[64] = {};
    ssize_t n = read(connfd, readbuf, sizeof(readbuf) - 1);
    if(n < 0){
        die("read() error");
        return;
    }

    std::cout << "client says: " << readbuf << std::endl;

    char writebuf[] = "world";
    write(connfd, writebuf, strlen(writebuf));
}

//client side--------------------------------------------------------------------
static int32_t query(int fd, const char *text){
    uint32_t len = (uint32_t)strlen(text);
    if(len > k_max_msg) return -1;
    
    //send request
    char writeBuffer[4 + k_max_msg];
    memcpy(writeBuffer, &len, 4);// little endian
    memcpy(&writeBuffer[4], text, len);
    if(int32_t err = write_all(fd, writeBuffer, 4 + len))return err;

    //4 byte header
    char readBuffer[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(fd, readBuffer, 4);
    if(err) {
        msg(errno == 0 ? "EOF" : "read() error");
        return err;
    }

    memcpy(&len, readBuffer, 4);// lil endian
    if (len > k_max_msg) {
        msg("too long");
        return -1;
    }
    //reply body
    err = read_full(fd, &readBuffer[4], len);
    if (err) {
        msg("read() error");
        return err;
    }

    std::cout << "server says :" << len << &readBuffer[4] << std::endl;
    return 0;
}
