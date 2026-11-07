#ifndef HTTP_RESPONSE_RESPONSE_HPP
#define HTTP_RESPONSE_RESPONSE_HPP

#include <string>

struct StatusLine {
  std::string http_version;
  int status_code;
  std::string reason_phrase;

  StatusLine(const std::string &version, int code, const std::string &reason) : http_version(version), status_code(code), reason_phrase(reason) {}
};

struct Header {
  std::string content_type;
  std::string content_length;
  std::string host;
  std::string user_agent;
  std::string accept;
};

struct Body {
  std::string type;
  std::string content;
};

class Response {
public:
  StatusLine status_line;
  Header header;
  Body body;

  Response(const StatusLine &status, const Header &hdr, const Body &bdy) : status_line(status), header(hdr), body(bdy) {}
  std::string ok();
  std::string not_found();
  std::string serialize();
};

#endif
