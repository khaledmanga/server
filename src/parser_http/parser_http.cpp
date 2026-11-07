#include "parser_http.hpp"

#include <charconv>
#include <limits>
#include <string_view>
#include <utility>

namespace {
bool parse_request_line(std::string_view line, Request &request) {
  const size_t method_end = line.find(' ');
  const size_t path_end = method_end == std::string_view::npos ? method_end : line.find(' ', method_end + 1);
  if (method_end == std::string_view::npos || method_end == 0 || path_end == std::string_view::npos || path_end == method_end + 1 ||
      path_end + 1 == line.size() || line.find(' ', path_end + 1) != std::string_view::npos) {
    return false;
  }

  const std::string_view version = line.substr(path_end + 1);
  const size_t slash = version.find('/');
  const size_t dot = slash == std::string_view::npos ? slash : version.find('.', slash + 1);
  if (slash == std::string_view::npos || slash == 0 || dot == std::string_view::npos || dot == slash + 1 || dot + 1 == version.size()) {
    return false;
  }

  int major = 0;
  int minor = 0;
  const char *version_data = version.data();
  const auto major_result = std::from_chars(version_data + slash + 1, version_data + dot, major);
  const auto minor_result = std::from_chars(version_data + dot + 1, version_data + version.size(), minor);
  if (major_result.ec != std::errc{} || major_result.ptr != version_data + dot || minor_result.ec != std::errc{} ||
      minor_result.ptr != version_data + version.size()) {
    return false;
  }

  request.request_line.method = std::string(line.substr(0, method_end));
  request.request_line.path = std::string(line.substr(method_end + 1, path_end - method_end - 1));
  request.request_line.protocol = std::string(version.substr(0, slash));
  request.request_line.major_version = major;
  request.request_line.minor_version = minor;
  return true;
}

bool parse_headers(std::string_view raw_headers, Request &request, size_t &content_length, bool &has_content_length, bool &has_transfer_encoding,
                   bool &is_chunked) {
  size_t position = 0;
  while (position < raw_headers.size()) {
    const size_t line_end = raw_headers.find("\r\n", position);
    const size_t end = line_end == std::string_view::npos ? raw_headers.size() : line_end;
    const std::string_view line = raw_headers.substr(position, end - position);
    const size_t colon = line.find(':');
    if (colon == std::string_view::npos || colon == 0) {
      return false;
    }

    size_t value_start = colon + 1;
    while (value_start < line.size() && (line[value_start] == ' ' || line[value_start] == '\t')) {
      ++value_start;
    }
    size_t value_end = line.size();
    while (value_end > value_start && (line[value_end - 1] == ' ' || line[value_end - 1] == '\t')) {
      --value_end;
    }

    std::string key(line.substr(0, colon));
    std::string value(line.substr(value_start, value_end - value_start));
    for (char &character : key) {
      if (character >= 'A' && character <= 'Z') {
        character = static_cast<char>(character - 'A' + 'a');
      }
    }

    if (key == "content-type") {
      request.header.content_type = value;
    } else if (key == "content-length") {
      size_t parsed_length = 0;
      const auto result = std::from_chars(value.data(), value.data() + value.size(), parsed_length);
      if (value.empty() || result.ec != std::errc{} || result.ptr != value.data() + value.size() || (has_content_length && content_length != parsed_length)) {
        return false;
      }
      content_length = parsed_length;
      has_content_length = true;
      request.header.content_length = value;
    } else if (key == "host") {
      request.header.host = value;
    } else if (key == "transfer-encoding") {
      for (char &character : value) {
        if (character >= 'A' && character <= 'Z') {
          character = static_cast<char>(character - 'A' + 'a');
        }
      }
      if (has_transfer_encoding || value != "chunked") {
        return false;
      }
      has_transfer_encoding = true;
      is_chunked = true;
    }

    if (line_end == std::string_view::npos) {
      break;
    }
    position = line_end + 2;
  }
  return !(has_content_length && has_transfer_encoding);
}

HttpParserState parse_chunked_body(std::string_view raw_body, Request &request) {
  size_t position = 0;
  std::string decoded_body;

  while (true) {
    const size_t line_end = raw_body.find("\r\n", position);
    if (line_end == std::string_view::npos) {
      return HttpParserState::BODY;
    }

    std::string_view size_text = raw_body.substr(position, line_end - position);
    const size_t extension = size_text.find(';');
    if (extension != std::string_view::npos) {
      size_text = size_text.substr(0, extension);
    }
    if (size_text.empty()) {
      return HttpParserState::ERROR;
    }

    size_t chunk_size = 0;
    const auto result = std::from_chars(size_text.data(), size_text.data() + size_text.size(), chunk_size, 16);
    if (result.ec != std::errc{} || result.ptr != size_text.data() + size_text.size()) {
      return HttpParserState::ERROR;
    }

    position = line_end + 2;
    if (chunk_size == 0) {
      if (raw_body.size() < position + 2) {
        return HttpParserState::BODY;
      }
      if (raw_body.substr(position, 2) == "\r\n") {
        request.body.type = "text/plain";
        request.body.content = std::move(decoded_body);
        return HttpParserState::COMPLETE;
      }

      const size_t trailers_end = raw_body.find("\r\n\r\n", position);
      if (trailers_end == std::string_view::npos) {
        return HttpParserState::BODY;
      }
      request.body.type = "text/plain";
      request.body.content = std::move(decoded_body);
      return HttpParserState::COMPLETE;
    }

    if (chunk_size > std::numeric_limits<size_t>::max() - position - 2) {
      return HttpParserState::ERROR;
    }
    if (raw_body.size() < position + chunk_size + 2) {
      return HttpParserState::BODY;
    }
    if (raw_body.substr(position + chunk_size, 2) != "\r\n") {
      return HttpParserState::ERROR;
    }

    decoded_body.append(raw_body.data() + position, chunk_size);
    position += chunk_size + 2;
  }
}
} // namespace

HttpParserState parse_http_request(const std::string &raw_request, Request &request) {
  const size_t request_line_end = raw_request.find("\r\n");
  if (request_line_end == std::string::npos) {
    return HttpParserState::REQUEST_LINE;
  }

  Request parsed_request{};
  if (!parse_request_line(std::string_view(raw_request).substr(0, request_line_end), parsed_request)) {
    return HttpParserState::ERROR;
  }

  const size_t headers_end = raw_request.find("\r\n\r\n", request_line_end);
  if (headers_end == std::string::npos) {
    return HttpParserState::HEADERS;
  }

  size_t content_length = 0;
  bool has_content_length = false;
  bool has_transfer_encoding = false;
  bool is_chunked = false;
  const size_t headers_start = request_line_end + 2;
  const std::string_view headers(raw_request.data() + headers_start, headers_end - headers_start);
  if (!parse_headers(headers, parsed_request, content_length, has_content_length, has_transfer_encoding, is_chunked)) {
    return HttpParserState::ERROR;
  }

  const size_t body_start = headers_end + 4;
  const std::string_view body(raw_request.data() + body_start, raw_request.size() - body_start);
  if (is_chunked) {
    const HttpParserState state = parse_chunked_body(body, parsed_request);
    if (state == HttpParserState::COMPLETE) {
      request = std::move(parsed_request);
    }
    return state;
  }

  if (has_content_length && body.size() < content_length) {
    return HttpParserState::BODY;
  }

  parsed_request.body.type = "text/plain";
  if (has_content_length) {
    parsed_request.body.content.assign(body.data(), content_length);
  }
  request = std::move(parsed_request);
  return HttpParserState::COMPLETE;
}
