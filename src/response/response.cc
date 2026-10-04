#include "response.h"

#include <sstream>

Response::Response() : status_code(HTTP::StatusCode::OK), content_type("text/plain; charset=utf-8") {}

Response &Response::status(HTTP::StatusCode status_code_) {
  this->status_code = status_code_;
  return *this;
}

Response &Response::send(const std::string &value) {
  this->body = value;
  this->content_type = "text/plain; charset=utf-8";
  return *this;
}

std::string Response::serialize() const {
  std::ostringstream response;
  response << "HTTP/1.1 " << static_cast<int>(status_code) << ' ' << HTTP::getReasonPhrase(status_code) << "\r\n"
           << "Content-Type: " << this->content_type << "\r\n"
           << "Content-Length: " << this->body.size() << "\r\n"
           << "Connection: close\r\n\r\n"
           << this->body;
  return response.str();
}

Response &Response::json(const nlohmann::json &value) {
  this->body = value.dump();
  this->content_type = "application/json";
  return *this;
}
