#pragma once

#include <string>
#include <sstream>
#include <optional>

#include "http/request/request.h"
#include "include/common.h"

HttpStatus http_parser(Request& , HttpStatus&, const std::string&);