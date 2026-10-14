#include "http_parser.h"

#include <charconv>
#include <system_error>

HttpStatus http_parser(Request& request, HttpStatus& http_status, const std::string& raw_request) {
    const size_t request_line_end = raw_request.find("\r\n");
    if(request_line_end == std::string::npos) {
        http_status = HttpStatus::RequestLine;
        return http_status;
    }

    std::istringstream request_line(raw_request.substr(0, request_line_end));
    std::string method;
    std::string path;
    std::string protocol_version;
    if(!(request_line >> method >> path >> protocol_version)) {
        http_status = HttpStatus::Error;
        return http_status;
    }

    const size_t slash = protocol_version.find('/');
    const size_t dot = protocol_version.find('.');
    if(slash == std::string::npos || dot == std::string::npos || dot <= slash + 1) {
        http_status = HttpStatus::Error;
        return http_status;
    }

    request.request_line.method = method;
    request.request_line.path = path;
    request.request_line.protocol = protocol_version.substr(0, slash);
    try {
        request.request_line.major_version =
            std::stoi(protocol_version.substr(slash + 1, dot - slash - 1));
        request.request_line.minor_version = std::stoi(protocol_version.substr(dot + 1));
    } catch(const std::exception&) {
        http_status = HttpStatus::Error;
        return http_status;
    }

    http_status = HttpStatus::Header;
    const size_t header_end = raw_request.find("\r\n\r\n", request_line_end);
    if(header_end == std::string::npos) {
        return http_status;
    }

    size_t line_start = request_line_end + 2;
    while(line_start < header_end) {
        const size_t line_end = raw_request.find("\r\n", line_start);
        if(line_end == std::string::npos || line_end > header_end) {
            http_status = HttpStatus::Error;
            return http_status;
        }

        const std::string header_line = raw_request.substr(line_start, line_end - line_start);
        const size_t colon = header_line.find(':');
        if(colon == std::string::npos) {
            http_status = HttpStatus::Error;
            return http_status;
        }

        size_t value_start = colon + 1;
        while(value_start < header_line.size() && header_line[value_start] == ' ') {
            ++value_start;
        }
        request.header.kv.insert_or_assign(
            header_line.substr(0, colon), header_line.substr(value_start));
        line_start = line_end + 2;
    }

    http_status = HttpStatus::Body;
    const size_t body_start = header_end + 4;
    const auto content_length = request.header["Content-Length"];
    if(content_length.has_value()) {
        size_t body_length;
        const char* length_start = content_length->data();
        const char* length_end = length_start + content_length->size();
        const auto result = std::from_chars(length_start, length_end, body_length);
        if(result.ec != std::errc{} || result.ptr != length_end) {
            http_status = HttpStatus::Error;
            return http_status;
        }
        if(raw_request.size() - body_start < body_length) {
            return http_status;
        }
        request.body.value = raw_request.substr(body_start, body_length);
    }

    http_status = HttpStatus::End;
    return http_status;
}