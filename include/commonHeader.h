#pragma once

#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#define PROTOCOL "HTTP/1.1"

// status codes
#define OK "200"
#define NO_CONTENT "204"
#define BAD_REQUEST "400"
#define FORIBIDDEN "403"
#define NOT_FOUND "404"
#define INTERNAL_SERVER_ERROR "500"
