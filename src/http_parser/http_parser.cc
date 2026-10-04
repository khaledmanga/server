#include "http_parser.h"

#include "../utils/common.h"

HttpRequestState httpParser(Request &request, HttpRequestState &httpRequestState, std::string &raw_request) {
  if (httpRequestState == HttpRequestState::Init) {
    httpRequestState = HttpRequestState::RequestLine;
  }

  if (httpRequestState == HttpRequestState::RequestLine) {
    auto clrf = raw_request.find("\r\n");

    if (clrf == std::string::npos) {
      return httpRequestState;
    }

    std::string request_line = raw_request.substr(0, clrf);

    std::istringstream iss(request_line);

    std::string method;
    std::string target;
    std::string protocol_version;

    iss >> method >> target >> protocol_version;

    if (!isValidMethodHttp(method)) {
      return httpRequestState;
    }

    request.request_line.method = method;
    request.request_line.target = target;

    auto slash = protocol_version.find('/');
    auto dot = protocol_version.find('.');

    request.request_line.protocol = protocol_version.substr(0, slash);
    request.request_line.major_version = std::stoi(protocol_version.substr(slash + 1, dot - slash - 1));
    request.request_line.minor_version = std::stoi(protocol_version.substr(dot + 1));

    httpRequestState = HttpRequestState::Headers;

    raw_request.erase(0, clrf + 2);
  }

  if (httpRequestState == HttpRequestState::Headers) {
    auto double_clrf = raw_request.find("\r\n\r\n");

    if (double_clrf == std::string::npos) {
      return httpRequestState;
    }

    std::string headers = raw_request.substr(0, double_clrf + 4);

    auto start = 0;
    auto crlf = headers.find("\r\n", start);

    while (crlf != std::string::npos && crlf <= double_clrf) {
      std::string field = headers.substr(start, crlf - start);

      auto colon = field.find(':');

      if (colon != std::string::npos) {
        std::string key = field.substr(0, colon);
        std::string value = field.substr(colon + 1);

        if (!value.empty() && value[0] == ' ') {
          value.erase(0, 1);
        }

        request.header.fields[key] = value;
      }

      start = crlf + 2;
      crlf = headers.find("\r\n", start);
    }

    httpRequestState = HttpRequestState::Body;

    raw_request.erase(0, double_clrf + 4);
  }

  if (httpRequestState == HttpRequestState::Body) {
    const auto content_length_header = request.header.fields.find("Content-Length");
    const std::size_t content_length = content_length_header == request.header.fields.end() ? 0 : std::stoul(content_length_header->second);

    if (content_length > 0) {
      if (raw_request.size() < content_length) {
        return httpRequestState;
      }

      request.body.content_type = request.header.fields["Content-Type"];
      request.body.value = raw_request.substr(0, content_length);
      raw_request.erase(0, content_length);
    }

    httpRequestState = HttpRequestState::Completed;
  }

  return httpRequestState;
}