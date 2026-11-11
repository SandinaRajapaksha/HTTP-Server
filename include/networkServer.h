
#include "commonHeader.h"
#include <cstdint>
#pragma once

#define SERVER_PORT 3490
#define SERVER_IPV4 "192.168.248.48"

class networkServer {
  private:
    int serverFD;
    int acceptFD;
    sockaddr_in serverAddr;

  public:
    networkServer();
    ~networkServer();

    int sendAll(std::string &message, std::vector<char> &httpBody);
    int startServer();
    void requestParse(std::vector<uint8_t> &);
    bool malformAndSecurityCheck();
    void route();
    void buildResponse();
};
