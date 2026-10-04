#include "request.h"

std::ostream &operator<<(std::ostream &os, const RequestLine &rl) {
  os << "Method: " << rl.method << std::endl
     << "Target: " << rl.target << std::endl
     << "Protocol: " << rl.protocol << "/" << rl.major_version << "." << rl.minor_version;

  return os;
}

std::ostream &operator<<(std::ostream &os, const Header &h) {
  for(auto const& [key, value]: h.fields) {
    os << key << ": " << value << std::endl;
  }

  return os;
}

std::ostream &operator<<(std::ostream &os, const Body &b) {
  os << "Body: " << b.value;

  return os;
}

std::ostream &operator<<(std::ostream &os, const Request &r) {
  os << r.request_line << std::endl << r.header << std::endl << r.body;

  return os;
}
