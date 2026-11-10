#include "commonHeader.h"

struct httpResponse {
    std::string sendableFullHeaderAndStatusLine;

    std::unordered_map<std::string, std::string> statusLine;
    std::unordered_map<std::string, std::string> headers;
    std::vector<uint8_t> body;

    httpResponse();
    void buildCompleteResponseHeader();
    void buildCompleteResponseBody();
};

struct httpRequest {
    std::unordered_map<std::string, std::string> statusLine;
    std::unordered_map<std::string, std::string> headers;

    std::vector<uint8_t> body;

    void parseThroughOriginalRequest();
};
