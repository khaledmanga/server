#include <string>

struct RequestLine {
  std::string method;
  std::string path;
  std::string protocol;
  int major_version;
  int minor_version;
};

struct Header {
  std::string content_type;
  std::string content_length;
  std::string host;
};

struct Body {
  std::string type;
  std::string content;
};

struct Request {
  RequestLine request_line;
  Header header;
  Body body;
};