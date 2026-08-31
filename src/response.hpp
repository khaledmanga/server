#pragma once

#include <cstddef>
#include <string>

std::string currentHttpDate();

class StatusLine {
public:
    StatusLine() = default;

    StatusLine& setVersion(std::string version);
    StatusLine& setStatusCode(int code);
    StatusLine& setReasonPhrase(std::string phrase);

    const std::string& version() const noexcept;
    int statusCode() const noexcept;
    const std::string& reasonPhrase() const noexcept;

    std::string serialize() const;

private:
    std::string version_ = "HTTP/1.1";
    int status_code_ = 200;
    std::string reason_phrase_ = "OK";
};

class Headers {
public:
    Headers();

    Headers& setContentType(std::string type);
    Headers& setContentLength(std::size_t length);
    Headers& setDate(std::string date);
    Headers& setServer(std::string server);
    Headers& setConnection(std::string connection);

    const std::string& contentType() const noexcept;
    std::size_t contentLength() const noexcept;
    const std::string& date() const noexcept;
    const std::string& server() const noexcept;
    const std::string& connection() const noexcept;

    std::string serialize() const;

private:
    std::string content_type_ = "application/json";
    std::size_t content_length_ = 0;
    std::string date_;
    std::string server_ = "MyCustomCppServer/1.0";
    std::string connection_ = "close";
};

class Response {
public:
    Response() = default;

    Response& setStatusLine(const StatusLine& status_line);
    Response& setHeaders(const Headers& headers);
    Response& setBody(std::string body);

    const StatusLine& statusLine() const noexcept;
    const Headers& headers() const noexcept;
    const std::string& body() const noexcept;

    std::string serialize() const;

private:
    StatusLine status_line_;
    Headers headers_;
    std::string body_;
};
