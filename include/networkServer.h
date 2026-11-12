#pragma once
#include "../include/commonHeader.h"
#include "../include/requestAndResponseHandling.h"
#include <cstdint>

#define SERVER_PORT 3000
#define SERVER_IPV4 "127.0.0.1"

class networkServer {
  private:
    int serverFD;
    int acceptFD;
    sockaddr_in serverAddr;

  public:
    networkServer();
    ~networkServer();

    int sendAll(std::string &message, std::vector<uint8_t> &httpBody);
    int startServer();
    void requestParse(std::vector<uint8_t> &);
    std::string malformAndSecurityCheck(httpRequest);
    void route(httpRequest, std::string);
    void buildResponse();
};
