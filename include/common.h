#pragma once

#include <string>

enum class HttpStatus {
    Start,
    RequestLine,
    Header,
    Body,
    End,
    Error,
};

enum class StatusCode {
    OK,
    NotFound,
};

int toStatusCode(StatusCode status_code);
std::string toStatusMessage(StatusCode status_code);