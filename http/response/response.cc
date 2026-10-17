#include "response.h"

std::string Response::serialize(const StatusCode &status_code) {
  std::string res =
      "HTTP/1.1 " + std::to_string(toStatusCode(status_code)) + " " + toStatusMessage(status_code) + "\r\n";

  for (auto const &[key, value] : this->header.kv) {
    res += key + ": " + value + "\r\n";
  }
  if (!this->header.kv.contains("Content-Length")) {
    res += "Content-Length: " + std::to_string(this->body.value.size()) + "\r\n";
  }

  res += "\r\n";

  res += this->body.value;
  return res;
}

std::string Response::serialize() { return this->serialize(this->status_code); }

std::string Response::ok() { return this->serialize(StatusCode::OK); }
std::string Response::not_found() { return this->serialize(StatusCode::NotFound); }