#pragma once

#include <cstdint>
#include <string>

enum class RequestState { RequestLine, Header, Body, Complete };

class RequestLine {
public:
    RequestLine() = default;

    RequestLine& setMethod(std::string method);
    RequestLine& setPath(std::string path);
    RequestLine& setVersion(uint8_t major_version, uint8_t minor_version);

    const std::string& method() const noexcept;
    const std::string& path() const noexcept;
    uint8_t majorVersion() const noexcept;
    uint8_t minorVersion() const noexcept;

private:
    std::string method_;
    std::string path_;
    uint8_t major_version_ = 1;
    uint8_t minor_version_ = 1;
};

class Header {
public:
    Header() = default;

    Header& setHost(std::string host);
    Header& setUserAgent(std::string user_agent);
    Header& setAccept(std::string accept);
    Header& setContentType(std::string content_type);
    Header& setContentLength(int content_length);

    const std::string& host() const noexcept;
    const std::string& userAgent() const noexcept;
    const std::string& accept() const noexcept;
    const std::string& contentType() const noexcept;
    int contentLength() const noexcept;

private:
    std::string host_;
    std::string user_agent_;
    std::string accept_;
    std::string content_type_;
    int content_length_ = 0;
};

class Request {
public:
    Request() = default;

    Request& setMethod(std::string method);
    Request& setPath(std::string path);
    Request& setVersion(uint8_t major_version, uint8_t minor_version);

    Request& setHeader(const Header& header);
    Request& setBody(std::string body);

    const std::string& method() const noexcept;
    const std::string& path() const noexcept;
    uint8_t majorVersion() const noexcept;
    uint8_t minorVersion() const noexcept;

    const Header& header() const noexcept;
    const std::string& body() const noexcept;

    void parseRequestLine(const std::string& request_line_raw);
    void parseHeader(const std::string& request_header_raw);
    bool parse(std::string& request_raw, RequestState& state);

    void printRequestLine() const;
    void printHeader() const;
    void printBody() const;

private:
    RequestLine request_line_;
    Header header_;
    std::string body_;
};
