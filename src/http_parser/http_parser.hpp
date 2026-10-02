#pragma once

#include <iostream>
#include <cstring>
#include <sstream>

#include "../request/request.hpp"
#include "../constant/common.hpp"

HttpRequestState httpParser(Request &request, HttpRequestState &httpRequestState, const std::string &raw_request);
