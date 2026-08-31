#include "request.hpp"

#include <iostream>
#include <sstream>
#include <utility>

RequestLine& RequestLine::setMethod(std::string method) {
    method_ = std::move(method);
    return *this;
}

RequestLine& RequestLine::setPath(std::string path) {
    path_ = std::move(path);
    return *this;
}

RequestLine& RequestLine::setVersion(uint8_t major_version,
                                     uint8_t minor_version) {
    major_version_ = major_version;
    minor_version_ = minor_version;
    return *this;
}

const std::string& RequestLine::method() const noexcept {
    return method_;
}

const std::string& RequestLine::path() const noexcept {
    return path_;
}

uint8_t RequestLine::majorVersion() const noexcept {
    return major_version_;
}

uint8_t RequestLine::minorVersion() const noexcept {
    return minor_version_;
}

Header& Header::setHost(std::string host) {
    host_ = std::move(host);
    return *this;
}

Header& Header::setUserAgent(std::string user_agent) {
    user_agent_ = std::move(user_agent);
    return *this;
}

Header& Header::setAccept(std::string accept) {
    accept_ = std::move(accept);
    return *this;
}

Header& Header::setContentType(std::string content_type) {
    content_type_ = std::move(content_type);
    return *this;
}

Header& Header::setContentLength(int content_length) {
    content_length_ = content_length;
    return *this;
}

const std::string& Header::host() const noexcept {
    return host_;
}

const std::string& Header::userAgent() const noexcept {
    return user_agent_;
}

const std::string& Header::accept() const noexcept {
    return accept_;
}

const std::string& Header::contentType() const noexcept {
    return content_type_;
}

int Header::contentLength() const noexcept {
    return content_length_;
}

Request& Request::setRequestLine(const RequestLine& request_line) {
    request_line_ = request_line;
    return *this;
}

Request& Request::setHeader(const Header& header) {
    header_ = header;
    return *this;
}

Request& Request::setBody(std::string body) {
    body_ = std::move(body);
    return *this;
}

const RequestLine& Request::requestLine() const noexcept {
    return request_line_;
}

const Header& Request::header() const noexcept {
    return header_;
}

const std::string& Request::body() const noexcept {
    return body_;
}

void Request::parseRequestLine(const std::string& request_line_raw) {
    std::string method;
    std::string path;
    std::string version;
    std::istringstream ss(request_line_raw);

    ss >> method >> path >> version;

    size_t pos_slash = version.find("/");
    size_t pos_dot = version.find(".");

    if (pos_slash == std::string::npos || pos_dot == std::string::npos) {
        return;
    }

    request_line_.setMethod(method);
    request_line_.setPath(path);

    request_line_.setVersion(
        static_cast<uint8_t>(
            std::stoi(version.substr(pos_slash + 1, pos_dot - pos_slash - 1))),
        static_cast<uint8_t>(std::stoi(version.substr(pos_dot + 1))));
}

void Request::parseHeader(const std::string& request_header_raw) {
    size_t pos_crlf = request_header_raw.find("\r\n");
    size_t start = 0;

    while (pos_crlf != std::string::npos) {
        std::string line = request_header_raw.substr(start, pos_crlf - start);
        size_t pos_colon = line.find(":");

        if (pos_colon != std::string::npos) {
            std::string key = line.substr(0, pos_colon);
            std::string value = line.substr(pos_colon + 1);

            if (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }

            if (key == "Host") {
                header_.setHost(value);
            } else if (key == "User-Agent") {
                header_.setUserAgent(value);
            } else if (key == "Accept") {
                header_.setAccept(value);
            } else if (key == "Content-Type") {
                header_.setContentType(value);
            } else if (key == "Content-Length") {
                header_.setContentLength(std::stoi(value));
            }
        }

        start = pos_crlf + 2;
        pos_crlf = request_header_raw.find("\r\n", start);
    }
}

bool Request::parse(std::string& request_raw, RequestState& state) {
    while (true) {
        if (state == RequestState::RequestLine) {
            size_t pos = request_raw.find("\r\n");

            if (pos == std::string::npos) {
                return false;
            }

            parseRequestLine(request_raw.substr(0, pos));

            request_raw.erase(0, pos + 2);

            state = RequestState::Header;
        } else if (state == RequestState::Header) {
            size_t pos = request_raw.find("\r\n\r\n");

            if (pos == std::string::npos) {
                return false;
            }

            parseHeader(request_raw.substr(0, pos + 2));

            request_raw.erase(0, pos + 4);

            state = RequestState::Body;
        } else if (state == RequestState::Body) {
            size_t len = header_.contentLength();

            if (request_raw.size() < len) {
                return false;
            }

            body_.assign(request_raw, 0, len);

            request_raw.erase(0, len);

            state = RequestState::Complete;
        } else if (state == RequestState::Complete) {
            return true;
        }
    }
}

void Request::printRequestLine() const {
    std::cout << "Method: " << request_line_.method() << std::endl;
    std::cout << "Path: " << request_line_.path() << std::endl;
    std::cout << "Version: HTTP/"
              << static_cast<int>(request_line_.majorVersion()) << "."
              << static_cast<int>(request_line_.minorVersion()) << std::endl;
}

void Request::printHeader() const {
    std::cout << "Host: " << header_.host() << std::endl;
    std::cout << "User-Agent: " << header_.userAgent() << std::endl;
    std::cout << "Accept: " << header_.accept() << std::endl;
    std::cout << "Content-Type: " << header_.contentType() << std::endl;
    std::cout << "Content-Length: " << header_.contentLength() << std::endl;
}

void Request::printBody() const {
    std::cout << "Body: " << body_ << std::endl;
}