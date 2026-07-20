#include "utilities.h"

int main(){
    const char* initialMessage = "[TCP CONNECTION ESTABLISHED MESSAGES ONGOING]";

    // creating the socket
    // fd = handle you will use later
    int fd = socket(AF_INET, SOCK_STREAM, 0); 
    //SOCK_STREAM => TCP
    if(fd < 0)serverLog("fd bad",2);
    else serverLog("fd good",1);

    int val = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    mySockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = htonl(0);

    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv) {serverLog("bind() bad",2);}
    else serverLog("bind() good",1);

    // listen
    rv = listen(fd, SOMAXCONN);
    if(rv) {serverLog("listen() bad",2);}
    else serverLog("listen() good",1);

    std::cout << initialMessage << '\n'; 
    
    //accept
    struct mySockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);// client len
    int connfd = accept(fd,(struct sockaddr *)&client_addr, &addrlen);
    
    if(connfd < 0){ serverLog("accept() bad",2);}
    else serverLog("accept() good",1);

    std::cout << "Type in messages to be sent \n";
    while(true){
        std::cout << "Server:";

        char userInputBuffer[1024] = {};// 1024 bytes
        std::string userInput;
        std::getline(std::cin, userInput);
        std::strcpy(userInputBuffer,userInput.c_str());

        int bytesSent = send(connfd, userInputBuffer, sizeof(userInputBuffer)-1,0);
        if(bytesSent < 0)serverLog("send() bad",2);

        if(userInput == "exit") break;
        
        char clientInputBuffer[1024] = {};
        int bytesRead = recv(connfd, clientInputBuffer,sizeof(clientInputBuffer)-1, 0);
        if(bytesRead < 0) serverLog("recv() bad",2);
        
        std::cout << "Client:" << clientInputBuffer << '\n';

        memset(userInputBuffer, 0, sizeof(userInputBuffer));
        memset(clientInputBuffer, 0, sizeof(clientInputBuffer));
    }

    close(connfd);
    
    return 0;
}