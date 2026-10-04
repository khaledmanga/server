#include "request.h"

std::ostream &operator<<(std::ostream &os, const RequestLine &rl) {
  os << "Method: " << rl.method << std::endl
     << "Target: " << rl.target << std::endl
     << "Protocol: " << rl.protocol << "/" << rl.major_version << "." << rl.minor_version;

  return os;
}

std::ostream &operator<<(std::ostream &os, const Header &h) {
  for (auto const &[key, value] : h.fields) {
    os << key << ": " << value << std::endl;
  }

  return os;
}

std::ostream &operator<<(std::ostream &os, const Body &b) {
  if (b.content_type == "application/json") {
    json json_value = json::parse(b.value);
    os << "Body: " << std::endl << json_value.dump(4);
  } else {
    os << "Body: " << b.value;
  }

  return os;
}

json Body::operator[](const std::string &key) {
  json json_value = json::parse(value);

  return json_value[key];
}

std::ostream &operator<<(std::ostream &os, const Request &r) {
  os << r.request_line << std::endl << r.header << std::endl << r.body;

  return os;
}
