#include "../include/networkServer.h"
#include "../include/commonHeader.h"
#include "../include/requestAndResponseHandling.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <vector>

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

        // recv loop
        std::vector<uint8_t> fullmessage;
        const int BUFFER_SIZE = 4096;
        uint8_t buffer[4096];

        int bytesRecieved = 1;
        while (bytesRecieved > 0) {

            bytesRecieved = 0;
            bytesRecieved = recv(acceptFD, buffer, sizeof(buffer), 0);
            if (bytesRecieved < 0) {
                std::cout << "recv() error" << std::endl;
                std::cout << strerror(errno) << std::endl;
            }
            // partial recv handling on last chunk
            if (bytesRecieved < BUFFER_SIZE) {
                for (int i = 0; i < bytesRecieved; i++) {
                    fullmessage.push_back(buffer[i]);
                }
            }
            // usual buffer append
            if (bytesRecieved == BUFFER_SIZE) {
                fullmessage.insert(fullmessage.end(), std::begin(buffer), std::end(buffer));
            }
        }
        // send message to the parser

        // httpResponse serverResponse;
        // serverResponse.statusLine["protocol"] = PROTOCOL;
        // serverResponse.statusLine["statusCode"] = "200";
        // serverResponse.statusLine["statusDesc"] = "OK";
        // serverResponse.headers["Content-Type"] = "text/html";
        //
        // serverResponse.buildCompleteResponseBody();
        // serverResponse.headers["Content-Length"] = std::to_string(serverResponse.body.size());
        // serverResponse.buildCompleteResponseHeader();
        //
        // sendAll(serverResponse.sendableFullHeaderAndStatusLine, serverResponse.body);
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

void networkServer::requestParse(std::vector<uint8_t> &RequestVector) {
    std::string fullRequest(RequestVector.begin(), RequestVector.end());
    httpResponse clientRequest;

    std::vector<std::string> tokens;
    std::string token;

    std::stringstream streamOfString(fullRequest);

    // splting on only ' '
    while (std::getline(streamOfString, token, ' ')) {
        tokens.push_back(token);
    }

    // status line
    clientRequest.statusLine["method"] = tokens[0];
    clientRequest.statusLine["path"] = tokens[1];
    clientRequest.statusLine["protocol"] = tokens[2];

    // header and body parser to be implemented later
}

std::string networkServer::malformAndSecurityCheck(httpRequest clientRequest) {
    for (auto i : clientRequest.statusLine) {
        if (i.second == "") {
            return BAD_REQUEST;
        }
    }

    // security checks - esacaping root fix
    // std::vector<std::string> pathTokens;
    // std::string pathToken;
    // std::string fullPathClientRequested = clientRequest.statusLine["requestedPath"];
    // std::stringstream streamOfPathString(fullPathClientRequested);
    //
    // while (std::getline(streamOfPathString, pathToken, '/')) {
    //     pathTokens.push_back(pathToken);
    // }
    //
    // for (auto i : pathTokens) {
    //     if (i == "..") {
    //         return FORIBIDDEN;
    //     }
    // }

    return OK;
}
