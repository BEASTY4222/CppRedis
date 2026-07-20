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
static void serverLog(const char* message, int type){
    if(type == 1)
        std::cout << "SERVERLOG::>" << message << "\n";
    else{
        std::cout << "SERVERLOG::>[ERROR]" << message << "\n";
        exit(-1);
    }      
}
static void clientLog(const char* message, int type){
    if(type == 1)
        std::cout << "CLIENTLOG::>" << message << "\n";
    else{
        std::cout << "CLIENTLOG::>[ERROR]" << message << "\n";
        exit(-1);
    }      
} 