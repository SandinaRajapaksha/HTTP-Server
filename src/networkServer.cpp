#include "../include/networkServer.h"
#include "../include/commonHeader.h"
#include "../include/requestAndResponseHandling.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
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
            if ((char)fullmessage.at(fullmessage.size() - 1) == '\n' &&
                (char)fullmessage.at(fullmessage.size() - 2) == '\r' &&
                (char)fullmessage.at(fullmessage.size() - 3) == '\n' &&
                (char)fullmessage.at(fullmessage.size() - 4) == '\r') {
                break;
            }
        }
        shutdown(acceptFD, SHUT_RD);
        requestParse(fullmessage);
        close(acceptFD);
    }

    return 0;
}

int networkServer::sendAll(std::string &statusLineAndHeaders, std::vector<uint8_t> &httpBody) {
    // body sending to be implemented
    std::string bodyMessage(httpBody.begin(), httpBody.end());
    int sentBytesCountHeader = send(acceptFD, statusLineAndHeaders.c_str(), statusLineAndHeaders.size(), 0);
    int sentBytesCountBody = send(acceptFD, bodyMessage.c_str(), bodyMessage.size(), 0);
    shutdown(acceptFD, SHUT_WR);
    return sentBytesCountHeader + sentBytesCountBody;
}

void networkServer::requestParse(std::vector<uint8_t> &RequestVector) {
    std::string fullRequest(RequestVector.begin(), RequestVector.end());
    httpRequest clientRequest;

    std::vector<std::string> tokens;
    std::string token;

    std::stringstream streamOfString(fullRequest);

    // splting on only ' '
    while (std::getline(streamOfString, token, ' ')) {
        tokens.push_back(token);
    }

    // status line
    clientRequest.statusLine["method"] = tokens[0];
    clientRequest.statusLine["requestedPath"] = WEB_ROOT + tokens[1];
    clientRequest.statusLine["protocol"] = tokens[2];

    // header and body parser to be implemented later
    std::string responseStatusCode = malformAndSecurityCheck(clientRequest);

    // send to route
    route(clientRequest, responseStatusCode);
}

std::string networkServer::malformAndSecurityCheck(httpRequest clientRequest) {
    for (auto i : clientRequest.statusLine) {
        if (i.second == "") {
            return BAD_REQUEST;
        }
    }
    if (clientRequest.statusLine["method"] != "GET") {
        return BAD_REQUEST;
    }

    // security checks - esacaping root fix
    return OK;
}

void networkServer::route(httpRequest clientRequest, std::string responseStatusCode) {
    if (responseStatusCode != OK) {
        errorSender(responseStatusCode);
        return;
    }
    if (clientRequest.statusLine["requestedPath"] == (webRoot + "/")) {
        // defalut serving file
        std::string defaultFilePath = webRoot + "/" + DEFAULT_FILE;
        std::ifstream file(defaultFilePath, std::ios::binary | std::ios::ate);
        std::vector<uint8_t> responseBodyVector = readAll(file);
        responseSender(responseBodyVector);

        // content type table to be implemented
    } else {

        std::ifstream file(clientRequest.statusLine["requestedPath"], std::ios::binary | std::ios::ate);
        bool fileOpened = file.good(); // Returns true if the file was opened successfully
        if (!fileOpened) {
            errorSender(NOT_FOUND);
            return;
        }
        std::vector<uint8_t> responseBodyVector = readAll(file);
        responseSender(responseBodyVector);
        // finding files
    }
}

void networkServer::errorSender(std::string responseStatusCode) {
    std::string reponseBody = "<html><body><h1>" + responseStatusCode + "<br>" + MACRO_NAME_TO_STR(responseStatusCode) + "</h1></body></html>";
    std::vector<uint8_t> responseBodyVector(reponseBody.begin(), reponseBody.end());

    httpResponse errorResponse;
    errorResponse.statusLine["protocol"] = PROTOCOL;
    errorResponse.statusLine["statusCode"] = responseStatusCode;
    errorResponse.statusLine["statusDesc"] = MACRO_NAME_TO_STR(responseStatusCode);

    errorResponse.headers["Content-Type"] = "text/html";
    errorResponse.headers["Content-Length"] = std::to_string(responseBodyVector.size());

    errorResponse.buildCompleteResponseBody(responseBodyVector);
    errorResponse.buildCompleteResponseHeader();

    sendAll(errorResponse.sendableFullHeaderAndStatusLine, errorResponse.body);
    return;
}

void networkServer::responseSender(std::vector<uint8_t> &responseBodyVector) {

    httpResponse serverResponse;
    serverResponse.statusLine["protocol"] = PROTOCOL;
    serverResponse.statusLine["statusCode"] = OK;
    serverResponse.statusLine["statusDesc"] = "OK";

    serverResponse.headers["Content-Type"] = "text/html";
    serverResponse.headers["Content-Length"] = std::to_string(responseBodyVector.size());

    serverResponse.buildCompleteResponseBody(responseBodyVector);
    serverResponse.buildCompleteResponseHeader();

    sendAll(serverResponse.sendableFullHeaderAndStatusLine, serverResponse.body);
    return;
}

std::vector<uint8_t> networkServer::readAll(std::ifstream &file) {
    if (!file) {
        std::cerr << "Failed to open file.\n";
    }
    // Get size and return cursor to the beginning
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate buffer and read raw bytes
    std::vector<uint8_t> buffer(size);
    if (file.read((char *)buffer.data(), size)) {
        std::cout << "Successfully read " << file.gcount() << " bytes.\n";
    }

    return buffer;
};
