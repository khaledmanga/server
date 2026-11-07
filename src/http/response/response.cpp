#include "response.hpp"

std::string Response::serialize() {
  const std::string content_type = !header.content_type.empty() ? header.content_type : body.type.empty() ? "text/plain" : body.type;

  const std::string &content = body.content;

  std::string response;
  response.reserve(status_line.http_version.size() + status_line.reason_phrase.size() + content_type.size() + content.size() + 64);

  response += status_line.http_version;
  response += " ";
  response += std::to_string(status_line.status_code);
  response += " ";
  response += status_line.reason_phrase;
  response += "\r\n";

  response += "Content-Type: ";
  response += content_type;
  response += "\r\n";

  response += "Content-Length: ";
  response += std::to_string(content.size());
  response += "\r\n";

  response += "\r\n";
  response += content;

  return response;
}

std::string Response::ok() {
  this->status_line = StatusLine("HTTP/1.1", 200, "OK");
  this->body.content = "<h1>Hello</h1>";
  this->header.content_type = "text/html; charset=utf-8";
  return this->serialize();
}

std::string Response::not_found() {
  this->status_line = StatusLine("HTTP/1.1", 404, "Not Found");
  this->body.content = "404 Not Found";
  this->header.content_type = "text/plain";
  return this->serialize();
}