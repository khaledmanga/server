#pragma once

#include <cstring>
#include <iostream>
#include <sstream>

#include "../constant/common.hpp"
#include "../request/request.hpp"
#include "../utils/common.hpp"

HttpRequestState httpParser(Request &request,
                            HttpRequestState &httpRequestState,
                            std::string &raw_request);
