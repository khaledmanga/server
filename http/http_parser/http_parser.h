#pragma once

#include <optional>
#include <sstream>
#include <string>

#include "http/request/request.h"
#include "include/common.h"

HttpStatus http_parser(Request &, HttpStatus &, std::string &);