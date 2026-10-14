#pragma once

#include <unordered_map>
#include <string>
#include <optional>

#include "include/common.h"

class RequestLine {
    public:
        std::string method;
        std::string path;
        std::string protocol;
        int major_version;
        int minor_version;
    };

class Header {
        public:
    std::unordered_map<std::string, std::string> kv;

    std::optional<std::string> operator[](const std::string& key) const;
};

class Body {
        public:
    std::string type;
    std::string value;
};

class Request {
        public:
    RequestLine request_line;
    Header header;
    Body body;
};