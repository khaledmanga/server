#include "response.hpp"

#include <ctime>
#include <utility>

std::string currentHttpDate() {

  char buffer[64]{};

  const std::time_t now = std::time(nullptr);

  const std::tm *utc_time = std::gmtime(&now);

  if (utc_time == nullptr) {
    return {};
  }

  if (std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT",
                    utc_time) == 0) {

    return {};
  }

  return buffer;
}

StatusLine &StatusLine::setVersion(std::string version) {

  version_ = std::move(version);

  return *this;
}

StatusLine &StatusLine::setStatusCode(int code) {

  status_code_ = code;

  return *this;
}

StatusLine &StatusLine::setReasonPhrase(std::string phrase) {

  reason_phrase_ = std::move(phrase);

  return *this;
}

const std::string &StatusLine::version() const noexcept { return version_; }

int StatusLine::statusCode() const noexcept { return status_code_; }

const std::string &StatusLine::reasonPhrase() const noexcept {

  return reason_phrase_;
}

std::string StatusLine::serialize() const {

  return version_ + " " + std::to_string(status_code_) + " " + reason_phrase_ +
         "\r\n";
}

Headers::Headers() : date_(currentHttpDate()) {}

Headers &Headers::setContentType(std::string type) {

  content_type_ = std::move(type);

  return *this;
}

Headers &Headers::setContentLength(std::size_t length) {

  content_length_ = length;

  return *this;
}

Headers &Headers::setDate(std::string date) {

  date_ = std::move(date);

  return *this;
}

Headers &Headers::setServer(std::string server) {

  server_ = std::move(server);

  return *this;
}

Headers &Headers::setConnection(std::string connection) {

  connection_ = std::move(connection);

  return *this;
}

const std::string &Headers::contentType() const noexcept {

  return content_type_;
}

std::size_t Headers::contentLength() const noexcept { return content_length_; }

const std::string &Headers::date() const noexcept { return date_; }

const std::string &Headers::server() const noexcept { return server_; }

const std::string &Headers::connection() const noexcept { return connection_; }

std::string Headers::serialize() const {

  std::string result;

  result += "Content-Type: " + content_type_ + "\r\n";

  result += "Content-Length: " + std::to_string(content_length_) + "\r\n";

  result += "Date: " + date_ + "\r\n";

  result += "Server: " + server_ + "\r\n";

  result += "Connection: " + connection_ + "\r\n";

  return result;
}

Response &Response::setStatusLine(const StatusLine &status_line) {

  status_line_ = status_line;

  return *this;
}

Response &Response::setHeaders(const Headers &headers) {

  headers_ = headers;

  return *this;
}

Response &Response::setBody(std::string body) {

  body_ = std::move(body);

  headers_.setContentLength(body_.size());

  return *this;
}

const StatusLine &Response::statusLine() const noexcept { return status_line_; }

const Headers &Response::headers() const noexcept { return headers_; }

const std::string &Response::body() const noexcept { return body_; }

std::string Response::serialize() const {

  return status_line_.serialize() + headers_.serialize() + "\r\n" + body_;
}