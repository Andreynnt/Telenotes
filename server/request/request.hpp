//
// Created by andreynt on 24.11.17.
//

#ifndef SERVER_REQUEST_HPP
#define SERVER_REQUEST_HPP

#include <string>
#include <vector>
#include "header.hpp"

namespace http {
    struct request {
        std::string method;
        std::string uri;
        int http_version_major;
        int http_version_minor;
        std::vector<header> headers;
    };
}

#endif //SERVER_REQUEST_HPP
