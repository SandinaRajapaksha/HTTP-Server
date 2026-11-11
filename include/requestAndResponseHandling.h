#include "commonHeader.h"

struct httpResponse {
    std::string sendableFullHeaderAndStatusLine;

    std::unordered_map<std::string, std::string> statusLine;
    std::unordered_map<std::string, std::string> headers;
    std::vector<char> body;

    httpResponse();
    void buildCompleteResponseHeader();
    void buildCompleteResponseBody();
};

struct httpRequest {
    std::unordered_map<std::string, std::string> statusLine;
    std::unordered_map<std::string, std::string> headers;
    std::vector<char> body;

    httpRequest();
    void parseThroughOriginalRequest();
};
