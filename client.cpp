#include "utilities.h"

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0) clientLog("socket() bad" ,2);
    else clientLog("socket() good",1);

    mySockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
    
    int rv = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv) clientLog("connect() bad", 2);
    else clientLog("connect() good",1);

    while(true){
        char serverInputBuffer[1024] = {};
        int bytesRead = read(fd, serverInputBuffer ,sizeof(serverInputBuffer) - 1);
        if(bytesRead < 0) clientLog("read() bad",2);

        std::cout << "Server:" << serverInputBuffer << std::endl;

        std::cout << "Client:";
        char userInputBuffer[1024] = {};// 1024 bytes
        std::string userInput;
        std::getline(std::cin, userInput);
        std::strcpy(userInputBuffer,userInput.c_str());

        
        int bytesSent = send(fd, userInputBuffer, sizeof(userInputBuffer)-1,0);
        if(bytesSent < 0)clientLog("send() bad",2);

        if(userInput == "exit") break;
    }
    
    close(fd);
    return 0;
}