#pragma once
#include "commonHeader.h"
#include <cstdint>

struct httpResponse {
    std::string sendableFullHeaderAndStatusLine;

    std::unordered_map<std::string, std::string> statusLine;
    std::unordered_map<std::string, std::string> headers;
    std::vector<uint8_t> body;

    httpResponse();
    void buildCompleteResponseHeader();
    void buildCompleteResponseBody(std::vector<uint8_t>);
};

struct httpRequest {
    std::unordered_map<std::string, std::string> statusLine;
    std::unordered_map<std::string, std::string> headers;
    std::vector<uint8_t> body;

    httpRequest();
    void parseThroughOriginalRequest();
};
