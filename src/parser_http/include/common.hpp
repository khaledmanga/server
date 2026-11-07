#ifndef COMMON_HPP
#define COMMON_HPP

enum class HttpParserState { START, REQUEST_LINE, HEADERS, BODY, COMPLETE, ERROR };

#endif