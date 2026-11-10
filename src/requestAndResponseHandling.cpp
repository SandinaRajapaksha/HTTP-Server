#include "../include/requestAndResponseHandling.h"

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

void httpResponse::buildCompleteResponseBody() {
    std::string message = "<html><h1>hello</h1> world </html>";
    std::vector<char> messageAsCharVector(message.begin(), message.end());
    body = messageAsCharVector;
}
