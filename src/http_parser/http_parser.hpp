#pragma once

#include <iostream>
#include <cstring>
#include <sstream>

#include "../request/request.hpp"
#include "../constant/common.hpp"
#include "../utils/common.hpp"

HttpRequestState httpParser(Request &request, HttpRequestState &httpRequestState, std::string &raw_request);
