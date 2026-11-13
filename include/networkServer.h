#pragma once
#include "../include/commonHeader.h"
#include "../include/requestAndResponseHandling.h"
#include <cstdint>

// configs
#define SERVER_PORT 3000
#define SERVER_IPV4 "127.0.0.1"
#define WEB_ROOT "/home/sandina/Documents/HTTP_server/HTTP-Server/publicHTML"
#define DEFAULT_FILE "index.html"

class networkServer {
  private:
    std::string webRoot = WEB_ROOT;
    std::map<std::string, std::string> allStatusDesc;
    std::map<std::string, std::string> contentType;

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
    void errorSender(std::string);
    std::vector<uint8_t> readAll(std::ifstream &file);
    void responseSender(std::string, std::vector<uint8_t> &);
};

std::map<std::string, std::string> contentTypeBuilder();
