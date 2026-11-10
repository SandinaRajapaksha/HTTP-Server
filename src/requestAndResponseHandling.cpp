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
}
