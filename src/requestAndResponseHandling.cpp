#include "../include/requestAndResponseHandling.h"

httpRequest::httpRequest() {
    statusLine["method"] = "";
    statusLine["requestedPath"] = "";
    statusLine["protocol"] = PROTOCOL;

    headers["Host"] = "";
    headers["User-Agent"] = "";
    headers["Accept"] = "";
    headers["Content-Type"] = "";
    headers["Authorization"] = "";
    headers["Cookie"] = "";
    headers["Accept-Encoding"] = "";
    headers["Cache-Control"] = "";
}
httpResponse::httpResponse() {
    statusLine["protocol"] = PROTOCOL;
    statusLine["statusCode"] = "";
    statusLine["statusDesc"] = "";

    headers["Access-Control-Allow-Origin"] = "";
    headers["Connection"] = "";
    headers["Content-Encoding"] = "";
    headers["Content-Type"] = "";
    headers["Date"] = "";
    headers["ETag"] = "";
    headers["Keep-Alive"] = "";
    headers["Last-Modified"] = "";
    headers["Server"] = "";
    headers["Set-Cookie"] = "";
    headers["Transfer-Encoding"] = "";
    headers["Vary"] = "";
}

void httpResponse::buildCompleteResponseHeader() {
    sendableFullHeaderAndStatusLine += (statusLine["protocol"] + " ");
    if (statusLine["statusDesc"] == "") {
        sendableFullHeaderAndStatusLine += (statusLine["statusCode"] + "\r\n");
    } else {
        sendableFullHeaderAndStatusLine += (statusLine["statusCode"] + " ");
        sendableFullHeaderAndStatusLine += (statusLine["statusDesc"] + "\r\n");
    }

    for (const auto &pair : headers) {
        if (pair.second == "") {
            continue;
        }
        sendableFullHeaderAndStatusLine += (pair.first + ": " + pair.second + "\r\n");
    }
    sendableFullHeaderAndStatusLine += "\r\n";
}

void httpResponse::buildCompleteResponseBody(std::vector<uint8_t> bodyVector) {
    body = bodyVector;
}

std::map<std::string, std::string> buildStatusDescs() {
    std::map<std::string, std::string> allStatusDesc;
    allStatusDesc[OK] = "OK";
    allStatusDesc[NO_CONTENT] = "NO CONTENT";
    allStatusDesc[BAD_REQUEST] = "BAD REQUEST";
    allStatusDesc[FORIBIDDEN] = "FORIBIDDEN";
    allStatusDesc[NOT_FOUND] = "NOT FOUND";
    allStatusDesc[INTERNAL_SERVER_ERROR] = "INTERNAL SERVER ERROR";

    return allStatusDesc;
}
