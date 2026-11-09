#include "../include/networkServer.h"
#include "../include/commonHeader.h"
#include <cerrno>
#include <cstring>
#include <iostream>
#include <sys/socket.h>

networkServer::networkServer() {
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0) {
        std::cout << strerror(errno) << std::endl;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    int ptnRTn = inet_pton(AF_INET, SERVER_IPV4, &serverAddr.sin_addr.s_addr);
    if (ptnRTn < 1) {
        std::cout << "error happened when storing server address" << std::endl;
    }
    std::memset(serverAddr.sin_zero, '\0', 8);
}

networkServer::~networkServer() {
    close(acceptFD);
    close(serverFD);
}

int networkServer::startServer() {
    int bindRtn = bind(serverFD, (sockaddr *)&serverAddr, sizeof(sockaddr_in));
    if (bindRtn < 0) {
        std::cout << "bind() error : " << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    int listenRtn = listen(serverFD, 15);
    if (listenRtn < 0) {
        std::cout << "listen() error : " << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Listening to port : " << SERVER_PORT << std::endl;

    acceptFD = accept(serverFD, NULL, NULL);
    if (acceptFD < 0) {
        std::cout << "accept() error" << std::endl;
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    // test message
    std::string message = "hello this is sandinas server\n";
    send(acceptFD, message.c_str(), message.size(), 0);
    shutdown(acceptFD, SHUT_WR);

    return 0;
}
