#include "../include/networkServer.h"
#include "../include/commonHeader.h"
#include "../include/requestAndResponseHandling.h"
#include <string>

networkServer::networkServer() {
    serverFD = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFD < 0) {
        std::cout << strerror(errno) << std::endl;
    }

    int opt = 1;
    setsockopt(serverFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    int ptnRTn = inet_pton(AF_INET, SERVER_IPV4, &serverAddr.sin_addr.s_addr);
    if (ptnRTn < 1) {
        std::cout << "error happened when storing server address" << std::endl;
    }
    std::memset(serverAddr.sin_zero, '\0', 8);
}

networkServer::~networkServer() {
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

    while (true) {
        acceptFD = accept(serverFD, NULL, NULL);
        if (acceptFD < 0) {
            std::cout << "accept() error" << std::endl;
            std::cout << strerror(errno) << std::endl;
            return -1;
        }
        // route and other logic
        httpResponse serverResponse;
        serverResponse.statusLine["protocol"] = PROTOCOL;
        serverResponse.statusLine["statusCode"] = "200";
        serverResponse.statusLine["statusDesc"] = "OK";
        serverResponse.headers["Content-Type"] = "text/html";

        serverResponse.buildCompleteResponseBody();
        serverResponse.headers["Content-Length"] = std::to_string(serverResponse.body.size());
        serverResponse.buildCompleteResponseHeader();

        sendAll(serverResponse.sendableFullHeaderAndStatusLine, serverResponse.body);
        close(acceptFD);
    }

    return 0;
}

int networkServer::sendAll(std::string &statusLineAndHeaders, std::vector<char> &httpBody) {

    std::string bodyMessage(httpBody.begin(), httpBody.end());
    int sentBytesCountHeader = send(acceptFD, statusLineAndHeaders.c_str(), statusLineAndHeaders.size(), 0);
    int sentBytesCountBody = send(acceptFD, bodyMessage.c_str(), bodyMessage.size(), 0);
    shutdown(acceptFD, SHUT_WR);
    return sentBytesCountHeader + sentBytesCountBody;
}
