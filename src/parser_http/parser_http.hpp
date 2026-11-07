#include "http/request/request.hpp"
#include "parser_http/include/common.hpp"

#include <string>

HttpParserState parse_http_request(const std::string &raw_request, Request &req);