#include "response.h"

#include <sstream>

Response::Response() : status_code(HTTP::StatusCode::OK) {}

Response &Response::status(HTTP::StatusCode status_code_) {
  this->status_code = status_code_;
  return *this;
}

Response &Response::send(const std::string &value) {
  this->body = value;
  return *this;
}

std::string Response::serialize() const {
  std::ostringstream response;
  response << "HTTP/1.1 " << static_cast<int>(this->status_code) << ' ' << HTTP::getReasonPhrase(this->status_code) << "\r\n"
           << "Content-Type: text/plain; charset=utf-8\r\n"
           << "Content-Length: " << this->body.size() << "\r\n"
           << "Connection: close\r\n\r\n"
           << this->body;
  return response.str();
}
