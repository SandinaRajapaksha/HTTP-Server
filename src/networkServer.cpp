#include "../include/networkServer.h"
#include "../include/commonHeader.h"

networkServer::networkServer() {
    serverFD = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(serverFD, SERVER_IPV4, &serverAddr.sin_addr.s_addr);
    memset(serverAddr.sin_zero, '\0', 8);
}

networkServer::~networkServer() {
    close(serverFD);
}
