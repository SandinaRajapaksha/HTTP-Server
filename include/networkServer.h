#include "commonHeader.h"
#define SERVER_PORT 3000
#define SERVER_IPV4 "192.168.248.8"

class networkServer {
  private:
    int serverFD;
    sockaddr_in serverAddr;

  public:
    networkServer();
    ~networkServer();
    int sendAll();
    int startServer();
};
