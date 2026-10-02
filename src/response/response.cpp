#include "response.hpp"

Response::Response(HTTP::StatusCode status_code) {
  this->status_line.protocol = "HTTP";
  this->status_line.major_version = 1;
  this->status_line.minor_version = 1;
  this->status_line.status_code = status_code;
  this->status_line.reasone = HTTP::getReasonPhrase(status_code);
}
