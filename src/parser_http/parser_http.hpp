#include "http/request/request.hpp"
#include "parser_http/include/common.hpp"

#include <sstream>
#include <string>

bool parse_request_line(const std::string &line, Request &request);
bool parse_header_line(const std::string &line, Request &request);
bool parse_body(const std::string &body, Request &request);
void parse_http_request(HttpParserState &parser_state, const std::string &raw_request, Request &request);