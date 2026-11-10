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
    for (const auto &pair : statusLine) {
        if (pair.first == "statusDesc") {
            sendableFullHeaderAndStatusLine += (pair.second + "\t\n");
            break;
        }
        if (statusLine["statusDesc"] == "" && pair.first == "statusCode") {
            sendableFullHeaderAndStatusLine += (pair.second + "\t\n");
            break;
        }
        sendableFullHeaderAndStatusLine += (pair.second + " ");
    }

    for (const auto &pair : headers) {
        if (pair.second == "") {
            continue;
        }
        sendableFullHeaderAndStatusLine += (pair.first + ": " + pair.second + "\t\n");
    }
    sendableFullHeaderAndStatusLine += "\t\n";
}
