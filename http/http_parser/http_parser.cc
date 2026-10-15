#include "http_parser.h"

HttpStatus http_parser(Request &request, HttpStatus &status, std::string &raw) {
  while (status != HttpStatus::End) {
    const size_t end = raw.find("\r\n");

    if (status == HttpStatus::RequestLine) {
      if (end == std::string::npos)
        return status;

      std::istringstream line(raw.substr(0, end));
      std::string version;
      auto &rl = request.request_line;
      line >> rl.method >> rl.path >> version;

      const size_t slash = version.find('/');
      const size_t dot = version.find('.');
      rl.protocol = version.substr(0, slash);
      rl.major_version = std::stoi(version.substr(slash + 1, dot - slash - 1));
      rl.minor_version = std::stoi(version.substr(dot + 1));

      raw.erase(0, end + 2);
      status = HttpStatus::Header;
    } else if (status == HttpStatus::Header) {
      if (end == std::string::npos)
        return status;

      if (end == 0) {
        status = HttpStatus::Body;
      } else {
        const std::string line = raw.substr(0, end);
        const size_t colon = line.find(':');
        const size_t value = line.find_first_not_of(' ', colon + 1);
        request.header.kv.insert_or_assign(line.substr(0, colon), line.substr(value));
      }
      raw.erase(0, end + 2);
    } else if (status == HttpStatus::Body) {
      const auto content_length = request.header["Content-Length"];
      if (content_length) {
        auto &body = request.body.value;
        const size_t total = std::stoull(*content_length);
        const size_t take = std::min(raw.size(), total - body.size());

        body.append(raw, 0, take);
        raw.erase(0, take);
        if (body.size() < total)
          return status;
      }
      status = HttpStatus::End;
    } else {
      return status;
    }
  }
  return status;
}