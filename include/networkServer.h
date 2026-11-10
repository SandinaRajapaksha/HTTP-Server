
#include "commonHeader.h"
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
    int sendAll();
    int startServer();
    void buildResponse();
};
